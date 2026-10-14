/**
 * Handheld fork: open the menu by holding Start + Select.
 *
 * On a handheld without the on-screen controller, Select alone is too easy to
 * hit by accident, and libultraship only lets the pad open the menu with
 * controller navigation turned on. Holding Start + Select for about a second
 * toggles the menu instead (once per press); B or Escape also close it.
 *
 *  - AndroidMenuCombo_Update(): every frame from SpaghettiGui::DrawMenu, with
 *    the pad state ImGui sees. Returns true once per press when the menu should
 *    be toggled.
 *  - AndroidMenuCombo_FilterButtons(): from read_controllers (src/main.c),
 *    keeps Start away from the game while Select is held with it, so the
 *    combination does not pause the race.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool AndroidMenuCombo_Update(bool start, bool back);
void AndroidMenuCombo_FilterButtons(uint16_t* buttons);

#ifdef __cplusplus
}
#endif
