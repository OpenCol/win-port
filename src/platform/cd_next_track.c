/* cd_next_track: candidate for 1078:0dd4 (260 bytes).  Verdict: RELOCATABLE.
 * symbol: cd_next_track
 *
 * Plays the next CD track: the "cdaudio" device is opened if none is open (device id kept
 * at DGROUP:0x3c7a), the current track and the track count are read (MCI_STATUS items 8 and
 * 3), and the track after the current one -- or track 1 past the last -- is played.
 * MSPlayCDTrack is defined here because the shipped call to it is the compiler's own
 * `push cs / call `.
 */

#include "colonization.h"

#include "w16mci.h"

void cd_next_track(void)
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
        if ((err = mciSendCommand(0, 0x803, 0x2000L, (unsigned long)(void *)&op)) != 0)
            return;
        dev = op.wDeviceID;
        g_cd_device = dev;
    }
    flags = 0x100;
    st.dwItem = 8;
    err = mciSendCommand(dev, 0x814, flags, (unsigned long)(void *)&st);
    if (err != 0)
        return;
    cur = st.dwReturn;
    flags = 0x100;
    st.dwItem = 3;
    err = mciSendCommand(dev, 0x814, flags, (unsigned long)(void *)&st);
    if (err != 0)
        return;
    flags = 8;
    if (cur + 1 > st.dwReturn)
        MSPlayCDTrack(1);
    else
        MSPlayCDTrack((unsigned char)cur + 1);
}
