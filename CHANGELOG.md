# Changelog — SpaghettiKart Handheld

Fork of [izzy2lost/SpaghettiKart](https://github.com/izzy2lost/SpaghettiKart) for Android handhelds with a built-in
controller. Tested on the AYN Odin 2 Portal (1920×1080, Android 13).

## 0.1 — 2026-10-14

Based on izzy2lost/SpaghettiKart `9253aad` (2026-08-02, app version 2.0.0).

### Changed
- Own app ID `com.izzy.kart.handheld`, app name "SpaghettiKart Handheld" — installs next to the original app.
- On-screen controller removed (stick, buttons, Menu button, toggle icon and the virtual SDL joystick behind them).
  The built-in controller goes through libultraship's normal mapping; touch still works as a mouse in the menus.
  The Mods button stays, shown while the menu is open, as the way to the mods and saves managers.
- Options menu: hold **Start + Select** for about a second to open or close it — exactly once per press, nothing
  more until both buttons are released. **B** closes it when no popup or input field inside it takes B. Select
  alone does nothing. While the menu is up the controller navigates it, also without the controller navigation
  setting. Start reaches the game after 200 ms and not at all when Select joins it, so the combination does not
  pause the race; a short tap is passed on.
- Real full screen: system bars hidden via `WindowInsetsController` (`setDecorFitsSystemWindows(false)`,
  transient bars by swipe, cutout short edges), also after dialogs and the file picker. No black strip at the
  bottom; the game renders at the full 1920×1080.

### Fixed
- The APK was missing Torch's extraction files (`config.yml`, `yamls/`, `meta/`): the asset staging task never ran,
  and the launcher stopped with "Could not unpack the bundled game files: config.yml" after choosing the ROM.
