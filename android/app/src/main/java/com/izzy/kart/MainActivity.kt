package com.izzy.kart

import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.FrameLayout
import org.libsdl.app.SDLActivity

/**
 * The game itself.
 *
 * Handheld fork: there is no on-screen controller — the game is played with the
 * device's built-in controller, and touch still works as a mouse in the menus.
 * The only thing drawn over the SDL surface is the button to the mods and saves
 * manager, shown while libultraship's menu is open.
 *
 * [LauncherActivity] guarantees mk64.o2r exists before this activity starts.
 */
class MainActivity : SDLActivity() {

    private lateinit var modsButton: Button

    private var menuOpen = false

    private val handler = Handler(Looper.getMainLooper())
    private val menuWatcher = object : Runnable {
        override fun run() {
            syncMenuState()
            handler.postDelayed(this, MENU_POLL_MS)
        }
    }

    // org/libsdl/app is kept byte-identical to the SDL release libultraship
    // pins, so the game library is named here rather than patched in there.
    // SDLActivity refuses to start if the Java glue and libSDL2.so disagree on
    // their version, so both have to move together.
    override fun getLibraries(): Array<String> = arrayOf("SDL2", "Spaghettify")

    private external fun isMenuOpen(): Boolean

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setupModsOverlay()
    }

    override fun onResume() {
        super.onResume()
        // Re-sync immediately as well as on the timer, so coming back from the
        // mods manager never leaves the button showing a stale menu state.
        syncMenuState()
        handler.removeCallbacks(menuWatcher)
        handler.postDelayed(menuWatcher, MENU_POLL_MS)
    }

    override fun onPause() {
        handler.removeCallbacks(menuWatcher)
        super.onPause()
    }

    override fun onDestroy() {
        handler.removeCallbacks(menuWatcher)
        super.onDestroy()
    }

    /**
     * Shows the Mods button while the engine's menu is open.
     *
     * The menu is opened and closed by the controller (Start + Select), a
     * keyboard, or the menu dismissing itself, so the engine is asked rather
     * than tracking it here.
     */
    private fun syncMenuState() {
        val open = runCatching { isMenuOpen() }.getOrDefault(false)
        if (open == menuOpen) return

        menuOpen = open
        // Managing mods is a settings action, so it stays out of reach until
        // the menu is up.
        modsButton.visibility = if (open) View.VISIBLE else View.GONE
    }

    private fun setupModsOverlay() {
        val inflater = getSystemService(LAYOUT_INFLATER_SERVICE) as LayoutInflater
        val overlay = inflater.inflate(R.layout.mods_overlay, null).apply {
            layoutParams = FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.MATCH_PARENT
            )
        }
        findViewById<ViewGroup>(android.R.id.content).addView(overlay)

        modsButton = overlay.findViewById(R.id.buttonMods)
        modsButton.setOnClickListener { startActivity(Intent(this, ModsActivity::class.java)) }
    }

    private companion object {
        /** Fast enough to feel immediate on a menu toggle, cheap enough to ignore. */
        const val MENU_POLL_MS = 150L
    }
}
