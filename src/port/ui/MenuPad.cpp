#include "MenuPad.h"

#include <imgui_internal.h>

#include <algorithm>

namespace MenuPad {
namespace {

// Owner of the keys taken from ImGui's navigation, so only this code sees them.
const ImGuiID kOwner = ImHashStr("MenuPad");

constexpr ImGuiKey kMenuButtons[] = {
    ImGuiKey_GamepadFaceUp, ImGuiKey_GamepadFaceLeft, ImGuiKey_GamepadL1,
    ImGuiKey_GamepadR1,     ImGuiKey_GamepadL2,       ImGuiKey_GamepadR2,
};

// Slider keys: left ones first, then right ones.
constexpr ImGuiKey kSliderKeys[] = {
    ImGuiKey_GamepadDpadLeft,
    ImGuiKey_GamepadLStickLeft,
    ImGuiKey_GamepadDpadRight,
    ImGuiKey_GamepadLStickRight,
};

// Hold-to-repeat: first repeat after kRepeatDelay, then every kRepeatStart seconds, getting faster by kRepeatAccel
// per step down to kRepeatMin. Still one step at a time, only quicker.
constexpr float kRepeatDelay = 0.35f;
constexpr float kRepeatStart = 0.12f;
constexpr float kRepeatAccel = 0.88f;
constexpr float kRepeatMin = 0.012f;

struct Repeat {
    float next = 0.0f;
    float interval = kRepeatStart;
};
Repeat sRepeat[IM_ARRAYSIZE(kSliderKeys)];

} // namespace

bool Active() {
    const ImGuiIO& io = ImGui::GetIO();
    return (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) && (io.BackendFlags & ImGuiBackendFlags_HasGamepad);
}

void ClaimMenuButtons() {
    if (!Active()) {
        return;
    }
    for (ImGuiKey key : kMenuButtons) {
        ImGui::SetKeyOwner(key, kOwner, ImGuiInputFlags_LockUntilRelease);
    }
}

bool Pressed(ImGuiKey key) {
    return Active() && ImGui::IsKeyPressed(key, ImGuiInputFlags_None, kOwner);
}

int SliderSteps(ImGuiID id) {
    if (!Active() || id == 0 || ImGui::GetFocusID() != id || ImGui::IsItemActive()) {
        return 0;
    }
    for (ImGuiKey key : kSliderKeys) {
        ImGui::SetKeyOwner(key, kOwner, ImGuiInputFlags_LockUntilRelease);
    }
    // A would put the slider into ImGui's own tweak mode, which steps by arbitrary amounts.
    ImGui::SetKeyOwner(ImGuiKey_GamepadFaceDown, kOwner, ImGuiInputFlags_LockUntilRelease);

    int steps = 0;
    for (int i = 0; i < IM_ARRAYSIZE(kSliderKeys); i++) {
        const int direction = i < IM_ARRAYSIZE(kSliderKeys) / 2 ? -1 : 1;
        if (!ImGui::IsKeyDown(kSliderKeys[i], kOwner)) {
            continue;
        }
        const float held = ImGui::GetKeyData(kSliderKeys[i])->DownDuration;
        Repeat& repeat = sRepeat[i];
        if (held <= 0.0f) {
            steps += direction;
            repeat.next = kRepeatDelay;
            repeat.interval = kRepeatStart;
            continue;
        }
        while (held >= repeat.next) {
            steps += direction;
            repeat.next += repeat.interval;
            repeat.interval = std::max(kRepeatMin, repeat.interval * kRepeatAccel);
        }
    }
    if (steps != 0) {
        ImGui::SetNavCursorVisible(true);
    }
    return steps;
}

void DrawNavCursor() {
    ImGuiContext& g = *GImGui;
    if (!Active() || !g.NavCursorVisible || g.NavId == 0 || g.NavWindow == nullptr || !g.NavIdIsAlive) {
        return;
    }
    ImRect rect = ImGui::WindowRectRelToAbs(g.NavWindow, g.NavWindow->NavRectRel[g.NavLayer]);
    rect.ClipWith(g.NavWindow->ClipRect);
    rect.Expand(4.0f);
    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    drawList->AddRectFilled(rect.Min, rect.Max, IM_COL32(255, 210, 40, 40), 5.0f);
    drawList->AddRect(rect.Min, rect.Max, IM_COL32(255, 210, 40, 255), 5.0f, 0, 4.0f);
}

} // namespace MenuPad
