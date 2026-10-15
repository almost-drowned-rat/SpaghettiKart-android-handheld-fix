# SpaghettiKart Handheld — Mario Kart 64 for Android handhelds (single screen)

Fork of [izzy2lost/SpaghettiKart](https://github.com/izzy2lost/SpaghettiKart), the Android port of
[SpaghettiKart](https://github.com/HarbourMasters/SpaghettiKart) (the Mario Kart 64 PC port), for Android handhelds
with a built-in controller. Tested on the **AYN Odin 2 Portal** (1920×1080, Android 13).

What this fork changes (details in [CHANGELOG.md](CHANGELOG.md)):

- Own app ID `com.izzy.kart.handheld` ("SpaghettiKart Handheld") — installs next to the original app
- No on-screen controller — play with the built-in controller; touch still works as a mouse in the menus. Only
  the **Mods** button (to the mods and saves managers) is shown while the menu is open
- Options menu on Android's **Back**: swipe from the edge to bring up the system bar and tap back (or use the back
  gesture); **B** closes it too. The game stands still while the menu is open
- Menu fully usable with the controller: D-pad / stick move, **A** select, **B** back / close, **L / R** section,
  **ZL / ZR** tab, **Left / Right** change a slider by one step, **Y** reset the selected setting to its default.
  Every setting has a reset button (orange when changed), and **Reset page** resets a whole page
- Real full screen, no black strip from the navigation bar; the game renders at the full 1920×1080
- Fixed: the APK was missing the files the launcher needs to turn the ROM into game data

The race renders in 16:9 by default (wider view, not stretched); title and menu screens keep their 4:3 borders.
For 4:3 in the race too, set *Settings → Graphics → Force aspect ratio* to *Original (4:3)*.

### AI assistance

The changes in this fork were made with AI assistance ([Claude Code](https://claude.com/claude-code)). Planning,
testing on the device and the decision what gets released are by the repository owner. Tested only on the
**AYN Odin 2 Portal** — use at your own risk. This applies only to the fork's own changes, not to the code of the
original projects.
