# SpaghettiKart Handheld — Mario Kart 64 for Android handhelds (single screen)

Fork of [izzy2lost/SpaghettiKart](https://github.com/izzy2lost/SpaghettiKart), the Android port of
[SpaghettiKart](https://github.com/HarbourMasters/SpaghettiKart) (the Mario Kart 64 PC port), for Android handhelds
with a built-in controller. Tested on the **AYN Odin 2 Portal** (1920×1080, Android 13).

What this fork changes (details in [CHANGELOG.md](CHANGELOG.md)):

- Own app ID `com.izzy.kart.handheld` ("SpaghettiKart Handheld") — installs next to the original app
- No on-screen controller — play with the built-in controller; touch still works as a mouse in the menus. Only
  the **Mods** button (to the mods and saves managers) is shown while the menu is open
- Options menu: hold **Start + Select** for about one second to open or close it (once per press); **B** closes
  it too. Start alone pauses the race as usual and does not reach the game while Select is held with it
- Real full screen, no black strip from the navigation bar; the game renders at the full 1920×1080
- Fixed: the APK was missing the files the launcher needs to turn the ROM into game data

The race renders in 16:9 by default (wider view, not stretched); title and menu screens keep their 4:3 borders.
For 4:3 in the race too, set *Settings → Graphics → Force aspect ratio* to *Original (4:3)*.

> ⚠️ **No game assets.** The APK on the [Releases](../../releases) page contains only this project's code, like the
> original releases. On the first start the app asks for your own ROM of **Mario Kart 64 (USA)**,
> SHA-1 `579c48e211ae952530ffc8738709f078d5dd215e`, and turns it into the game data on the device.

Branch `handheld` is the fork's branch; `main` stays at izzy2lost's port. For playing, controls, mods and the
desktop versions see the [original README](https://github.com/HarbourMasters/SpaghettiKart#readme).

## Building

Requires a Linux host (WSL works) with cmake, ninja, gcc/g++, JDK 17 and the Android SDK with
`platforms;android-36`, `ndk;30.0.15729638` and `cmake;3.30.3`.

```bash
git clone --recurse-submodules -b handheld https://github.com/almost-drowned-rat/SpaghettiKart-android-handheld-fix.git
cd SpaghettiKart-android-handheld-fix
# 1) Torch on the host, then the port's own assets (no ROM needed)
cmake -S torch -B torch/build-host -G Ninja -DCMAKE_BUILD_TYPE=Release -DUSE_STANDALONE=ON -DBUILD_MK64=ON \
  -DBUILD_NAUDIO=ON -DBUILD_SM64=OFF -DBUILD_SF64=OFF -DBUILD_FZERO=OFF -DBUILD_MARIO_ARTIST=OFF
cmake --build torch/build-host --parallel
mkdir -p android/app/src/main/assets
torch/build-host/torch pack assets android/app/src/main/assets/spaghetti.o2r o2r
# 2) APK
cd android && ./gradlew assembleDebug
```

Release signing reads `android/key.properties` (gitignored) or the environment, see `android/app/build.gradle.kts`.

## Credits and license

- [izzy2lost/SpaghettiKart](https://github.com/izzy2lost/SpaghettiKart) — the Android port this fork is based on
  (Kotlin app, on-device ROM extraction, mods and saves managers)
- [HarbourMasters/SpaghettiKart](https://github.com/HarbourMasters/SpaghettiKart) — SpaghettiKart, the Mario Kart 64
  port, and its contributors
- [libultraship](https://github.com/Kenix3/libultraship) (MIT) and [Torch](https://github.com/HarbourMasters/Torch)
  (MIT) by the Harbour Masters
- [GreenNaugahyde/ZeldaWWHDRecompAndroid](https://github.com/GreenNaugahyde/ZeldaWWHDRecompAndroid) — full-screen
  approach (`hideSystemBars`, MPL-2.0), via the Wind Waker and DK64 handheld forks
- [SDL2](https://www.libsdl.org/), [Dear ImGui](https://github.com/ocornut/imgui) and the other libraries in the
  submodules

SpaghettiKart has no license file of its own (neither upstream nor in izzy2lost's port); this fork does not add
one. libultraship and Torch are MIT-licensed. The changes of this fork are in this repository.

*Fan project, not affiliated with Nintendo. Mario Kart 64 is © Nintendo.*
