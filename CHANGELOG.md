# Changelog — SpaghettiKart Handheld

Fork of [izzy2lost/SpaghettiKart](https://github.com/izzy2lost/SpaghettiKart) for Android handhelds with a built-in
controller. Tested on the AYN Odin 2 Portal (1920×1080, Android 13).

## 0.2 — 2026-10-15

### Changed
- Options menu on Android's **Back** instead of holding Start + Select: swipe from the edge to bring up the system
  bar and tap its back button (or use the back gesture) to open or close the menu; **B** closes it too. Nothing
  else in the game uses Back. Start reaches the game immediately again (no more 200 ms delay).
- The game stands still while the menu is open, however it was opened: race, timers, opponents and music stop, the
  last picture stays behind the menu, and closing it resumes exactly there. Buttons still held when the menu closes
  (e.g. B) do not reach the game until released, so closing it does not brake or pick an entry.
- Menu fully usable with the controller, always on while the menu is open (the "Menu Controller Navigation" setting
  is hidden on Android): D-pad / left stick move, **A** selects, **B** goes back or closes, **L / R** switch the
  sidebar section, **ZL / ZR** the header tab, **Left / Right** on a slider change it by one step (held: repeating
  faster, still one step at a time), **Y** resets the selected setting. The D-pad moves across header, sidebar and
  columns like one page. A thick yellow frame marks the selected entry, which starts on the first entry; a footer
  line lists the buttons. Tooltips of the selected entry appear after a short delay. Touch keeps working alongside.
- Reset-to-default button next to every setting, orange while the value differs from the default (grey
  otherwise); resetting applies at once (e.g. the internal resolution). **Reset page** in the footer resets the
  whole sidebar page after a confirmation.
- Sliders snap to whole steps (internal resolution and volumes to whole percent, also values saved by 0.1) and
  show - / + buttons and their value.

### Fixed
- The game started with the menu open when it had been open on quitting (its visibility is saved with the
  settings); it now always starts with the menu closed.
- B closed the whole menu when it was only meant to close an open drop-down list.
- Default values that did not match what the game uses: Vsync (on), No LOD (on), texture filter (three-point).

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
