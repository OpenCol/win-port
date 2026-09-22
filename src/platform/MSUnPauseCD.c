/* MSUnPauseCD -- shipped at 1078:0d1d.
 *
 * Ported by tools/port.py from matched/platform/MSUnPauseCD.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_cd_device;                         DGROUP:0x3c7a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal mciSendCommand(col_u16 dev, col_u16 msg, col_u32 flags, col_u32 parms);
#endif

#include "w16mci.h"
   /* 16 bytes in this build */



void MSUnPauseCD(void)
{
    MCI_GENERIC_PARMS gp;
    col_u32 flags;
    col_u32 r;
    MCI_STATUS_PARMS st;
    MCI_PLAY_PARMS pp;

    flags = 0x100L;
    st.dwItem = 4L;
    r = mciSendCommand(g_cd_device, 0x814, flags, (col_u32)(void *)&st);
    if (r != 0)
        return;
    if (st.dwReturn == 0x20dL) {
        flags = 0;
        r = mciSendCommand(g_cd_device, 0x806, flags, (col_u32)(void *)&pp);
        if (r != 0)
            return;
    } else if (st.dwReturn == 0x211L) {
        flags = 0;
        r = mciSendCommand(g_cd_device, 0x855, flags, (col_u32)(void *)&gp);
    }
}
