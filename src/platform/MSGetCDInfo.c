/* MSGetCDInfo -- shipped at 1078:0fd7.
 *
 * Ported by tools/port.py from matched/platform/MSGetCDInfo.c.
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
#define MCI_TMSF_TRACK(t) ((unsigned char)(t))
#define MCI_TMSF_MINUTE(t) ((unsigned char)(((col_u16)(t)) >> 8))
#define MCI_TMSF_SECOND(t) ((unsigned char)((t) >> 16))
   /* 16 bytes in this build */



col_i16 MSGetCDInfo(col_i16 *minutes, col_i16 *seconds, col_i16 *tracks)
{
    col_u32 flags;
    col_u32 r;
    MCI_STATUS_PARMS st;
    MCI_SET_PARMS sp;
    col_u16 dev;

    if (g_cd_device == 0)
        return 0;
    else
        dev = g_cd_device;
    sp.dwTimeFormat = 10L;
    flags = 0x400L;
    r = mciSendCommand(dev, 0x80d, flags, (col_u32)(void *)&sp);
    if (r != 0)
        return 0;
    flags = 0x100L;
    st.dwItem = 2L;
    r = mciSendCommand(dev, 0x814, flags, (col_u32)(void *)&st);
    if (r != 0)
        return 0;
    if (minutes != 0)
        *minutes = MCI_TMSF_MINUTE(st.dwReturn);
    if (seconds != 0)
        *seconds = MCI_TMSF_SECOND(st.dwReturn);
    if (tracks != 0)
        *tracks = MCI_TMSF_TRACK(st.dwReturn);
    return 1;
}
