/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 30, 31 of the imported file. */
/* PORT: this file's own declarations of get_win_1068_d72a, mci_play, win_set_focus_1068_e278 removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 31, 32 of the imported file. */
/* audio_play_element: candidate for 1068:d5b3 (75 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Plays the MCI element `audio_open_element` (1068:d560) parked at +0x8c, then takes the
 * input focus back. `mci_play` is the MCI_PLAY send -- command 0x806 with
 * MCI_NOTIFY and the window's HWND as `dwCallback` -- so the window handed to it is the one
 * the device notifies when the clip ends, and `win_set_focus_1068_e278` is what undoes the
 * focus that gives away.
 *
 * The guard reads the whole 32-bit handle (`CMP DWORD [es:bx+0x8c],0`), not its offset
 * half, exactly as the opener's success test does.
 *
 * Both helpers are reached by the four-byte `PUSH CS / CALL NEAR` form, so both are in this
 * translation unit and appear here as stubs (rule 47); the MCI send is a call and is
 * `extern`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */

void audio_play_element(Audio *o)
{
    if (o->mci != 0) {
        mci_play((MSWin *)(get_win_1068_d72a((Screen *)((Audio *)((Screen *)o)))), (MciDev *)o->mci);
        win_set_focus_1068_e278((DialogWin *)((Audio *)((DialogWin *)o)));
    }
}
