/* MSGetCDTracks -- shipped at 1078:09cf.
 *
 * Ported by tools/port.py from matched/platform/MSGetCDTracks.c.
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


col_i16 MSGetCDTracks(void)
{
    col_i16 n;
    col_u32 r;
    MCI_OPEN_PARMS op;
    MCI_SET_PARMS sp;
    MCI_STATUS_PARMS st;
    col_u16 dev;

    op.lpstrDeviceType = "cdaudio";
    if ((r = mciSendCommand(0, 0x803, 0x2000L, (col_u32)(void *)&op)) != 0)
        return -1;
    dev = op.wDeviceID;
    sp.dwTimeFormat = 2L;
    if ((r = mciSendCommand(dev, 0x80d, 0x400L, (col_u32)(void *)&sp)) != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        return -1;
    }
    st.dwItem = 3L;
    if ((r = mciSendCommand(dev, 0x814, 0x100L, (col_u32)(void *)&st)) != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        return -1;
    }
    n = (col_i16)st.dwReturn;
    if ((r = mciSendCommand(dev, 0x804, 0L, 0L)) != 0)
        return -1;
    return n;
}
