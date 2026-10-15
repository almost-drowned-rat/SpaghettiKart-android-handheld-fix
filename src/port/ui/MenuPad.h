#ifndef MENU_PAD_H
#define MENU_PAD_H

/**
 * Controller handling for the menu, on top of Dear ImGui's gamepad navigation.
 *
 * ImGui's own mapping moves with the D-pad / left stick, activates with A and goes back with B. This adds what a
 * handheld without mouse or keyboard needs:
 *  - L / R switch the sidebar section, ZL / ZR the header tab;
 *  - Left / Right on a highlighted slider change its value by exactly one step per press, repeating faster while
 *    held (ImGui would otherwise need A first and then tweak by an arbitrary amount);
 *  - a thick, well visible highlight around the selected entry;
 *  - Y and X are kept away from ImGui (it would open text input or its window switcher with them).
 *
 * Everything is a no-op while gamepad navigation is off (desktop default).
 */

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

namespace MenuPad {

// Gamepad navigation is on (always while the menu is open on Android).
bool Active();
// Once per frame while the menu is drawn: takes Y, X, L, R, ZL and ZR from ImGui's navigation.
void ClaimMenuButtons();
// A claimed button was pressed this frame (no repeat).
bool Pressed(ImGuiKey key);
// For a slider whose ImGui item is `id`: while it is highlighted, takes Left / Right (and A) from ImGui's navigation
// and returns how many steps to move this frame, negative for left. One step per press, then repeating faster.
int SliderSteps(ImGuiID id);
// After the menu is drawn: the highlight around the selected entry.
void DrawNavCursor();

} // namespace MenuPad

#endif
