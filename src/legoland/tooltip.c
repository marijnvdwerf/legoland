#include "tooltip.h"
#include <windows.h>
#include "globals.h"
#include "legoland.h"

#include "image_sprite.h"
#include "stream.h"

// FUNCTION: LEGOLAND 0x0046d340
void ShowHelpText(const char *param_1) {
    if (SpeechIsPlaying() == 0) {
        if (param_1 == 0) {
            return;
        }
        if ((UINT_PTR)param_1 != CurrentHelpKey) {
            CurrentHelpKey = (UINT_PTR)param_1;
            HelpShownTick = GetTickCount();
            CurrentHelpKind = 2;
            HelpChanged = 1;
        }
    }
    HelpRequested = 1;
}

/* Sets CurrentHelpKind to 5 when interval report/hint speech starts; UpdateSpeechPlayback only replays kinds 0-2,
 * so hover help does not restart it. */
// FUNCTION: LEGOLAND 0x0046d390
void SetIntervalSpeechHelpKind(void) {
    CurrentHelpKind = 5;
}
