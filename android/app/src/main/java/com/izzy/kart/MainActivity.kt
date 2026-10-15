package com.izzy.kart

import android.content.Intent
import android.os.Build
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.view.WindowInsets
import android.view.WindowInsetsController
import android.view.WindowManager
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
        fillScreen()
        setupModsOverlay()
    }

    override fun onResume() {
        super.onResume()
        fillScreen()
        // Re-sync immediately as well as on the timer, so coming back from the
        // mods manager never leaves the button showing a stale menu state.
        syncMenuState()
        handler.removeCallbacks(menuWatcher)
        handler.postDelayed(menuWatcher, MENU_POLL_MS)
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        // Also after a dialog, the file picker or another app had the screen.
        if (hasFocus) fillScreen()
    }

    /**
     * Full screen without the system bars (handheld fork, as in the DK64 and
     * Wind Waker forks; adapted from GreenNaugahyde's MainActivity.hideSystemBars,
     * MPL-2.0). SDL only sets the old setSystemUiVisibility flags, which left the
     * navigation bar's space as a black strip at the bottom on the AYN Odin 2
     * Portal and cut off the bottom of the picture. A swipe from the edge shows
     * the bars for a moment.
     */
    private fun fillScreen() {
        val window = window ?: return
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            window.attributes = window.attributes.apply {
                layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES
            }
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            window.setDecorFitsSystemWindows(false)
            window.insetsController?.let {
                it.hide(WindowInsets.Type.systemBars())
                it.systemBarsBehavior = WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
            }
        } else {
            @Suppress("DEPRECATION")
            window.decorView.systemUiVisibility = (View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                or View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                or View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                or View.SYSTEM_UI_FLAG_FULLSCREEN)
        }
    }

    private val rehideBars = Runnable { fillScreen() }

    // Bars shown by something else (SDL's window style, the system): hidden
    // again after a moment.
    @Deprecated("Deprecated in Android, still called by SDL's listener")
    override fun onSystemUiVisibilityChange(visibility: Int) {
        @Suppress("DEPRECATION")
        super.onSystemUiVisibilityChange(visibility)
        @Suppress("DEPRECATION")
        if (visibility and View.SYSTEM_UI_FLAG_HIDE_NAVIGATION == 0 && hasWindowFocus()) {
            window.decorView.removeCallbacks(rehideBars)
            window.decorView.postDelayed(rehideBars, 2000)
        }
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
     * The menu is opened and closed by Android's Back (system bar or gesture),
     * B, a keyboard, or the menu dismissing itself, so the engine is asked
     * rather than tracking it here.
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
