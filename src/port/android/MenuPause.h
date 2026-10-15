/**
 * Handheld fork: the game stands still while the menu is open.
 *
 * Whichever way the menu was opened (Back, touch, keyboard), the game
 * logic, its timers and the sound stop for as long as it is up; the last frame
 * stays on screen behind it. Closing it resumes exactly where it stopped.
 *
 *  - AndroidMenuPause_Update(): once per frame from push_frame (Game.cpp).
 *    Returns true while the game should stand still.
 *  - AndroidMenuPause_FilterPad(): from read_controllers (src/main.c). Buttons
 *    and the stick that are still held when the menu closes (B, A, a
 *    direction) do not reach the game until they are released, so closing the
 *    menu does not pause the race or pick a menu entry.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool AndroidMenuPause_Update(void);
void AndroidMenuPause_FilterPad(int index, uint16_t* buttons, int8_t* stickX, int8_t* stickY);

#ifdef __cplusplus
}
#endif
