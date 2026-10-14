/**
 * Handheld fork: open the menu by holding Start + Select. See MenuCombo.h.
 */
#ifdef __ANDROID__

#include "MenuCombo.h"

#include <android/log.h>

#include <atomic>
#include <chrono>

namespace {

using Clock = std::chrono::steady_clock;

// How long Start + Select must be held.
constexpr auto kHoldTime = std::chrono::milliseconds(1000);
// Start reaches the game only after this delay, so Select may follow it
// without the game seeing a pause. Two buttons pressed "together" are often
// 100-150 ms apart, so 100 ms was too short.
constexpr auto kStartDelay = std::chrono::milliseconds(200);
// A tap of Start shorter than kStartDelay is passed on for this long after the
// release, so the game still sees it.
constexpr auto kStartPulse = std::chrono::milliseconds(100);

// N64 START (libultraship's CONT_START).
constexpr uint16_t kN64Start = 0x1000;

// Written by the UI side, read by the input side.
std::atomic<bool> gBackHeld{ false };

// UI side.
Clock::time_point gComboSince{};
bool gComboHeld = false;
bool gArmed = true; // only fires again after both buttons were released

// Input side.
Clock::time_point gStartOnset{};
Clock::time_point gPulseUntil{};
bool gStartDown = false;
bool gStartTainted = false;   // Select came with this Start press
bool gStartForwarded = false; // Start already reached the game during this press

} // namespace

extern "C" bool AndroidMenuCombo_Update(bool start, bool back) {
    gBackHeld.store(back, std::memory_order_relaxed);
    const auto now = Clock::now();

    // One toggle per press: after firing, nothing happens until both buttons
    // are released, so neither holding on nor letting go toggles again.
    if (!start && !back) {
        gArmed = true;
    }

    if (!(start && back)) {
        gComboHeld = false;
        return false;
    }

    if (!gComboHeld) {
        gComboHeld = true;
        gComboSince = now;
    }

    if (gArmed && now - gComboSince >= kHoldTime) {
        gArmed = false;
        __android_log_print(ANDROID_LOG_INFO, "SpaghettiKart", "menu combo: Start + Select held for %lld ms",
                            (long long)std::chrono::duration_cast<std::chrono::milliseconds>(now - gComboSince).count());
        return true;
    }
    return false;
}

extern "C" void AndroidMenuCombo_FilterButtons(uint16_t* buttons) {
    const bool start = (*buttons & kN64Start) != 0;
    const bool back = gBackHeld.load(std::memory_order_relaxed);
    const auto now = Clock::now();
    bool forward = false;

    if (start) {
        if (!gStartDown) {
            gStartDown = true;
            gStartOnset = now;
            gStartTainted = false;
            gStartForwarded = false;
        }
        if (back) {
            gStartTainted = true;
        }
        forward = !gStartTainted && now - gStartOnset >= kStartDelay;
        gStartForwarded |= forward;
    } else if (gStartDown) {
        gStartDown = false;
        if (!gStartTainted && !gStartForwarded) {
            gPulseUntil = now + kStartPulse;
        }
    }

    if (!back && now < gPulseUntil) {
        forward = true;
    }

    if (forward) {
        *buttons |= kN64Start;
    } else {
        *buttons &= ~kN64Start;
    }
}

#endif
