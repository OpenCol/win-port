/* cd_prev_track -- shipped at 1078:0ed8.
 *
 * Ported by tools/port.py from matched/platform/cd_prev_track.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_cd_device;                         DGROUP:0x3c7a
 *   extern unsigned g_cd_notify_hwnd;                    DGROUP:0x5726
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i32 MSPlayCDTrack(unsigned char track);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal mciSendCommand(col_u16 dev, col_u16 msg, col_u32 flags, col_u32 parms);
#endif

#include "w16mci.h"




extern void MSCloseCD(void);
extern void mm_error_report(void *ctx, char *msg, col_i32 code);



void cd_prev_track(void)
{
    col_u32 flags;
    col_u32 err;
    col_u32 cur;
    MCI_STATUS_PARMS st;
    MCI_OPEN_PARMS op;
    col_u16 dev;

    if (g_cd_device != 0)
        dev = g_cd_device;
    else {
        op.lpstrDeviceType = "cdaudio";
        if ((err = mciSendCommand(0, 0x803, 0x2000L, (col_u32)(void *)&op)) != 0)
            return;
        dev = op.wDeviceID;
        g_cd_device = dev;
    }
    flags = 0x100;
    st.dwItem = 8;
    err = mciSendCommand(dev, 0x814, flags, (col_u32)(void *)&st);
    if (err != 0)
        return;
    cur = st.dwReturn;
    flags = 0x100;
    st.dwItem = 3;
    err = mciSendCommand(dev, 0x814, flags, (col_u32)(void *)&st);
    if (err != 0)
        return;
    flags = 8;
    if (cur == 1)
        MSPlayCDTrack(1);
    else
        MSPlayCDTrack((unsigned char)(cur - 1));
}
