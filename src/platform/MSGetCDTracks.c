/* PORT: mciSendCommand's parameter block is passed as uintptr_t, not unsigned long: 8 bytes on 64-bit Windows (W9). */
/* MSGetCDTracks: candidate for 1078:09cf (292 bytes).  Verdict: RELOCATABLE.
 *
 * Opens "cdaudio" temporarily, sets MSF time format, asks MCI_STATUS_NUMBER_OF_TRACKS
 * and closes the device again; -1 on any failure, else the track count.
 */

#include "colonization.h"

#include "w16mci.h"
   /* 16 bytes in this build */

int MSGetCDTracks(void)
{
    int n;
    unsigned long r;
    MCI_OPEN_PARMS op;
    MCI_SET_PARMS sp;
    MCI_STATUS_PARMS st;
    unsigned dev;

    op.lpstrDeviceType = "cdaudio";
    if ((r = mciSendCommand(0, 0x803, 0x2000L, (uintptr_t)(void *)&op)) != 0)
        return -1;
    dev = op.wDeviceID;
    sp.dwTimeFormat = 2L;
    if ((r = mciSendCommand(dev, 0x80d, 0x400L, (uintptr_t)(void *)&sp)) != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        return -1;
    }
    st.dwItem = 3L;
    if ((r = mciSendCommand(dev, 0x814, 0x100L, (uintptr_t)(void *)&st)) != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        return -1;
    }
    n = (int)st.dwReturn;
    if ((r = mciSendCommand(dev, 0x804, 0L, 0L)) != 0)
        return -1;
    return n;
}
