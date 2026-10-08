/* PORT: mciSendCommand's parameter block is passed as uintptr_t, not unsigned long: 8 bytes on 64-bit Windows (W9). */
/* mci_play: candidate for 1078:2f60 (76 bytes).  Verdict: RELOCATABLE.
 *
 * Starts an MCI device playing and asks to be told when it stops:
 * `mciSendCommand(dev->id, MCI_PLAY, MCI_NOTIFY, &parms)` with `parms.dwCallback` set to
 * the window handle. That window is `mci_notify_wndproc`'s -- this is the call
 * that arms the `MM_MCINOTIFY` the other function handles, and the two are the ends of one
 * mechanism.
 *
 * `dwFrom` and `dwTo` are both zero, so it always plays the whole thing.
 *
 * TWO SPELLINGS THE FRAME AND THE ZERO FIX.
 *
 * The zeroing is a CHAINED assignment: `p.dwFrom = p.dwTo = 0;`. The shipped code clears
 * `EAX` once and stores it twice, where two separate `= 0` statements emit two 32-bit
 * immediate stores and run four bytes longer. The chain also fixes the ORDER -- `dwTo` is
 * stored first, because it is the inner assignment.
 *
 * And the `MCI_NOTIFY` flag is a LOCAL, not the constant: `SUB SP,0x10` is 16 bytes for a
 * 12-byte parameter block plus a four-byte flag word, and the flag sits at [BP-6] ABOVE
 * the block at [BP-0x12], so it is declared first. Passing `1` directly needs no local and
 * the frame comes out four bytes shorter. Rule 32 -- the frame states the declaration
 * order -- reading a `DWORD` rather than a word.
 */

#include "colonization.h"

#include "w16mci.h"
#include "records.h"

void mci_play(MSWin *win, MciDev *mci)
{
    unsigned long flags;
    MCI_PLAY_PARMS p;

    p.dwCallback = (unsigned)win->hwnd;
    p.dwFrom = p.dwTo = 0;
    flags = 1;
    mciSendCommand(mci->id, 0x806, flags, (uintptr_t)(void *)&p);
}
