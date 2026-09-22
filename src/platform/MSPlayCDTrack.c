/* MSPlayCDTrack -- shipped at 1078:0718.
 *
 * Ported by tools/port.py from matched/platform/MSPlayCDTrack.c.
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
void mm_error_report(void *ctx, char *msg, col_i32 code);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal mciSendCommand(col_u16 dev, col_u16 msg, col_u32 flags, col_u32 parms);
#endif

#include "w16mci.h"




extern void MSCloseCD(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

col_i32 MSPlayCDTrack(unsigned char track)
{
    col_u32 r;
    MCI_SEEK_PARMS sk;
    col_u32 flags;
    col_i32 tracks;
    MCI_OPEN_PARMS op;
    MCI_SET_PARMS sp;
    MCI_PLAY_PARMS pp;
    MCI_STATUS_PARMS st;
    col_u16 dev;

    if (g_cd_device != 0)
        MSCloseCD();
    op.lpstrDeviceType = "cdaudio";
    r = mciSendCommand(0, 0x803, 0x2000L, (col_u32)(void *)&op);
    if (r != 0) {
        mm_error_report(&(*g_error_ctx), "Failed to open CDAUDIO device", (col_i32)(col_i16)r);
        return 0;
    }
    dev = op.wDeviceID;
    g_cd_device = dev;
    sp.dwTimeFormat = 10L;
    r = mciSendCommand(dev, 0x80d, 0x400L, (col_u32)(void *)&sp);
    if (r != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        mm_error_report(&(*g_error_ctx), "CDAUDIO: could not set time format", (col_i32)(col_i16)r);
        return 0;
    }
    st.dwCallback = 0;
    st.dwReturn = 0;
    st.dwTrack = 0;
    st.dwItem = 3;
    mciSendCommand(dev, 0x814, 0x102L, (col_u32)(void *)&st);
    tracks = (col_i16)st.dwReturn;
    sk.dwCallback = 0;
    sk.dwTo = (col_u16)track;
    r = mciSendCommand(dev, 0x807, 0xaL, (col_u32)(void *)&sk);
    if (r != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        mm_error_report(&(*g_error_ctx), "Failed to play requested CD Track", (col_i32)(col_i16)r);
        return 0;
    }
    if ((col_u32)track == tracks)
        flags = 5L;
    else
        flags = 0xdL;
    pp.dwFrom = 0;
    pp.dwTo = 0;
    pp.dwFrom = (col_u16)track;
    pp.dwTo = (col_u16)(unsigned char)(track + 1);
    pp.dwCallback = g_cd_notify_hwnd;
    r = mciSendCommand(dev, 0x806, flags, (col_u32)(void *)&pp);
    if (r != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        mm_error_report(&(*g_error_ctx), "Failed to play requested CD Track", (col_i32)(col_i16)r);
        return 0;
    }
    return 1;
}
