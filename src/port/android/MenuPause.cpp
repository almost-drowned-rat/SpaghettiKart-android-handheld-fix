/**
 * Handheld fork: the game stands still while the menu is open. See MenuPause.h.
 */
#ifdef __ANDROID__

#include "MenuPause.h"

#include <android/log.h>

#include <cstdlib>

#include "ship/Context.h"
#include "ship/window/Window.h"
#include "ship/window/gui/Gui.h"

namespace {

constexpr int kPads = 4;
// The stick counts as released below this deflection (raw range about ±80).
constexpr int kStickRest = 16;

bool gPaused = false;

// Set when the menu closes, cleared per button / stick once released.
uint16_t gHeldButtons[kPads] = {};
bool gHeldStick[kPads] = {};
bool gLatchPending[kPads] = {};

bool MenuIsOpen() {
    auto* context = Ship::Context::GetRawInstance();
    if (context == nullptr || context->GetWindow() == nullptr) {
        return false;
    }
    auto gui = context->GetWindow()->GetGui();
    if (gui == nullptr) {
        return false;
    }
    auto menu = gui->GetMenu();
    return menu != nullptr && menu->IsVisible();
}

} // namespace

extern "C" bool AndroidMenuPause_Update(void) {
    const bool open = MenuIsOpen();
    if (open != gPaused) {
        gPaused = open;
        if (!open) {
            // Whatever is held on the first frame after closing gets latched.
            for (int i = 0; i < kPads; i++) {
                gLatchPending[i] = true;
            }
        }
        __android_log_print(ANDROID_LOG_INFO, "SpaghettiKart", "menu pause: game %s", open ? "paused" : "resumed");
    }
    return gPaused;
}

extern "C" void AndroidMenuPause_FilterPad(int index, uint16_t* buttons, int8_t* stickX, int8_t* stickY) {
    if (index < 0 || index >= kPads) {
        return;
    }
    const bool stickAtRest = std::abs(*stickX) < kStickRest && std::abs(*stickY) < kStickRest;

    if (gLatchPending[index]) {
        gLatchPending[index] = false;
        gHeldButtons[index] = *buttons;
        gHeldStick[index] = !stickAtRest;
    }

    gHeldButtons[index] &= *buttons;
    *buttons &= ~gHeldButtons[index];

    if (gHeldStick[index] && stickAtRest) {
        gHeldStick[index] = false;
    }
    if (gHeldStick[index]) {
        *stickX = 0;
        *stickY = 0;
    }
}

#endif
