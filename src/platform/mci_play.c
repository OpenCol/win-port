/* mci_play -- shipped at 1078:2f60.
 *
 * Ported by tools/port.py from matched/platform/mci_play.c.
 *
 * The file's own MSWin, MciDev dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal mciSendCommand(col_u16 id, col_u16 msg, col_u32 p1,
                                          col_u32 p2);
#endif

#include "w16mci.h"
void mci_play(MSWin *w, MciDev *d)
{
    col_u32 flags;
    MCI_PLAY_PARMS p;

    p.dwCallback = (col_u16)w->hwnd;
    p.dwFrom = p.dwTo = 0;
    flags = 1;
    mciSendCommand(d->id, 0x806, flags, (col_u32)(void *)&p);
}
