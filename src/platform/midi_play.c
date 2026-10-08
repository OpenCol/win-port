/* PORT: mciSendCommand's parameter block is passed as uintptr_t, not unsigned long: 8 bytes on 64-bit Windows (W9). */
/* midi_play: candidate for 1078:0594 (388 bytes).  Verdict: RELOCATABLE.
 *
 * Plays a MIDI file: any open device (DGROUP:0x3c78) is stopped, the "sequencer" device
 * opened on the file (id kept at 0x3c78), its output port checked (status item 0x4003; a
 * port that is not the MIDI Mapper asks "The MIDI Mapper is not available. Continue?" and No
 * closes the device), and MCI_PLAY issued with notification to the multimedia window
 * (DGROUP:0x5726). Every MCI failure is reported through mm_error_report and its
 * code returned; success returns 0. The two helper calls are calls TLINK converted.
 */

#include "colonization.h"

#include "w16mci.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

long midi_play(const char *name)
{
    unsigned long err;
    MCI_OPEN_PARMS op;
    MCI_PLAY_PARMS pp;
    MCI_STATUS_PARMS st;
    unsigned dev;

    if (g_mci_device != 0)
        stop_mci_device_1078_0940();
    op.lpstrDeviceType = "sequencer";
    op.lpstrElementName = name;
    err = mciSendCommand(0, 0x803, 0x2200L, (uintptr_t)(void *)&op);
    if (err != 0) {
        mm_error_report(&g_error_log, "Midi Device failed to open", (long)(int)err);
        return err;
    }
    dev = op.wDeviceID;
    g_mci_device = dev;
    st.dwItem = 0x4003;
    err = mciSendCommand(dev, 0x814, 0x100L, (uintptr_t)(void *)&st);
    if (err != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        mm_error_report(&g_error_log, "Output port is not MIDI Mapper", (long)(int)err);
        return err;
    }
    if ((long)(unsigned)st.dwReturn != -1L) {
        if (MessageBox(0, "The MIDI Mapper is not available. Continue?", "", 4) == 7) {
            mciSendCommand(dev, 0x804, 0L, 0L);
            return 0;
        }
    }
    pp.dwCallback = (unsigned long)g_cd_notify_hwnd;
    err = mciSendCommand(dev, 0x806, 1L, (uintptr_t)(void *)&pp);
    if (err != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        mm_error_report(&g_error_log, "Midi Play error", (long)(int)err);
        return err;
    }
    return 0;
}
