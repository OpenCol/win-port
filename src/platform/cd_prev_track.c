/* PORT: mciSendCommand's parameter block is passed as uintptr_t, not unsigned long: 8 bytes on 64-bit Windows (W9). */
/* cd_prev_track: candidate for 1078:0ed8 (255 bytes).  Verdict: RELOCATABLE.
 * symbol: cd_prev_track
 *
 * Plays the previous CD track: the "cdaudio" device is opened if none is open (device id
 * kept at DGROUP:0x3c7a), the current track and the track count are read (MCI_STATUS items 8
 * and 3), and the track before the current one is played -- track 1 stays on track 1.
 * MSPlayCDTrack is defined here because the shipped call to it is the compiler's own
 * `push cs / call `.
 */

#include "colonization.h"

#include "w16mci.h"

void cd_prev_track(void)
{
    unsigned long flags;
    unsigned long err;
    unsigned long cur;
    MCI_STATUS_PARMS st;
    MCI_OPEN_PARMS op;
    unsigned dev;

    if (g_cd_device != 0)
        dev = g_cd_device;
    else {
        op.lpstrDeviceType = "cdaudio";
        if ((err = mciSendCommand(0, 0x803, 0x2000L, (uintptr_t)(void *)&op)) != 0)
            return;
        dev = op.wDeviceID;
        g_cd_device = dev;
    }
    flags = 0x100;
    st.dwItem = 8;
    err = mciSendCommand(dev, 0x814, flags, (uintptr_t)(void *)&st);
    if (err != 0)
        return;
    cur = st.dwReturn;
    flags = 0x100;
    st.dwItem = 3;
    err = mciSendCommand(dev, 0x814, flags, (uintptr_t)(void *)&st);
    if (err != 0)
        return;
    flags = 8;
    if (cur == 1)
        MSPlayCDTrack(1);
    else
        MSPlayCDTrack((unsigned char)(cur - 1));
}
