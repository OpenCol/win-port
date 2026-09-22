/* midi_play -- shipped at 1078:0594.
 *
 * Ported by tools/port.py from matched/platform/midi_play.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_mci_device;                        DGROUP:0x3c78
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
extern col_i16 pascal MessageBox(col_u16 hwnd, const char *text, const char *title, col_u16 type);
extern col_u32 pascal mciSendCommand(col_u16 dev, col_u16 msg, col_u32 flags, col_u32 parms);
#endif

#include "w16mci.h"





extern void stop_mci_device_1078_0940(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

col_i32 midi_play(const char *name)
{
    col_u32 err;
    MCI_OPEN_PARMS op;
    MCI_PLAY_PARMS pp;
    MCI_STATUS_PARMS st;
    col_u16 dev;

    if (g_mci_device != 0)
        stop_mci_device_1078_0940();
    op.lpstrDeviceType = "sequencer";
    op.lpstrElementName = name;
    err = mciSendCommand(0, 0x803, 0x2200L, (col_u32)(void *)&op);
    if (err != 0) {
        mm_error_report(&(*g_error_ctx), "Midi Device failed to open", (col_i32)(col_i16)err);
        return err;
    }
    dev = op.wDeviceID;
    g_mci_device = dev;
    st.dwItem = 0x4003;
    err = mciSendCommand(dev, 0x814, 0x100L, (col_u32)(void *)&st);
    if (err != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        mm_error_report(&(*g_error_ctx), "Output port is not MIDI Mapper", (col_i32)(col_i16)err);
        return err;
    }
    if ((col_i32)(col_u16)st.dwReturn != -1L) {
        if (MessageBox(0, "The MIDI Mapper is not available. Continue?", "", 4) == 7) {
            mciSendCommand(dev, 0x804, 0L, 0L);
            return 0;
        }
    }
    pp.dwCallback = (col_u32)g_cd_notify_hwnd;
    err = mciSendCommand(dev, 0x806, 1L, (col_u32)(void *)&pp);
    if (err != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        mm_error_report(&(*g_error_ctx), "Midi Play error", (col_i32)(col_i16)err);
        return err;
    }
    return 0;
}
