/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 29 of the imported file. */
/* audio_open_element: candidate for 1068:d560 (83 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 83, not the census 77. The trailing 6 bytes are the dead second epilogue.
 *
 * Opens an MCI element for the object and parks the handle at +0x8c: 1 when it opened, 0
 * when it did not. `audio_play_element`, 83 bytes below, is the counterpart that tests that same
 * slot before handing it to `mci_play`, so this is where the slot is filled.
 *
 * `mci_open_element(get_win_1068_d72a(o), name)` -- the first argument is the window the
 * object holds at +6 (get_win_1068_d72a), not the object.
 *
 * The success test reads the whole 32-bit handle (`CMP DWORD [es:bx+0x8c],0`), not its
 * offset half.
 *
 * The `else` is load-bearing (rule 43): it puts a `JMP` after the `return 1` epilogue and a
 * third epilogue at the end for it to land on. Without it the function is six bytes shorter.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void *get_win_1068_d72a(Audio *o);
extern void *mci_open_element(void *win, char *name);

int audio_open_element(Audio *o, char *name)
{
    o->mci = mci_open_element((MSWin *)(get_win_1068_d72a((Audio *)((Screen *)o))), name);
    if (o->mci != 0)
        return 1;
    else
        return 0;
}
