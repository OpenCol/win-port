/* super_win_set_open: candidate for 1050:6be1 (26 bytes).  Verdict: RELOCATABLE.
 *
 * Raises the super-window-open flag at DGROUP:0x251e when its second argument is non-zero, and
 * does nothing when it is zero. The first argument is never read: it is four bytes wide, which
 * is what puts the tested word at [bp+0xa], so the signature is a pointer this function
 * ignores and a flag it acts on.
 *
 * ITS SIBLING READS WHAT THIS WRITES. `SuperWin_LeftMouseDown`, the very next function,
 * sets DGROUP:0x2520 only `if (g_super_win_open == 0)`, and is one of three button handlers of
 * `SuperWin_Init`'s dialog. The two words are that dialog's state: 0x251e whether the super
 * window is up, 0x2520 whether the modal is finished.
 */

#include "colonization.h"

void super_win_set_open(void *w, int open)
{
    if (open != 0)
        g_super_win_open = 1;
}
