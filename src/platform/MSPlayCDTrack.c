/* MSPlayCDTrack: candidate for 1078:0718 (552 bytes).  Verdict: RELOCATABLE.
 *
 * Plays one CD track: any open device is closed, "cdaudio" opened (device id kept at
 * DGROUP:0x3c7a), TMSF time format set, the track count read, the head seeked to the
 * track, and MCI_PLAY issued from the track to the next one -- the last track with flag 5,
 * others with 0xd, notifying the window at DGROUP:0x5726. Every MCI failure but the track-count MCI_STATUS (whose result is never checked) is reported with
 * its code and returns 0; success returns 1.
 */

#include "colonization.h"

#include "w16mci.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

long MSPlayCDTrack(unsigned char track)
{
    unsigned long r;
    MCI_SEEK_PARMS sk;
    unsigned long flags;
    long tracks;
    MCI_OPEN_PARMS op;
    MCI_SET_PARMS sp;
    MCI_PLAY_PARMS pp;
    MCI_STATUS_PARMS st;
    unsigned dev;

    if (g_cd_device != 0)
        MSCloseCD();
    op.lpstrDeviceType = "cdaudio";
    r = mciSendCommand(0, 0x803, 0x2000L, (unsigned long)(void *)&op);
    if (r != 0) {
        mm_error_report(&g_error_log, "Failed to open CDAUDIO device", (long)(int)r);
        return 0;
    }
    dev = op.wDeviceID;
    g_cd_device = dev;
    sp.dwTimeFormat = 10L;
    r = mciSendCommand(dev, 0x80d, 0x400L, (unsigned long)(void *)&sp);
    if (r != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        mm_error_report(&g_error_log, "CDAUDIO: could not set time format", (long)(int)r);
        return 0;
    }
    st.dwCallback = 0;
    st.dwReturn = 0;
    st.dwTrack = 0;
    st.dwItem = 3;
    mciSendCommand(dev, 0x814, 0x102L, (unsigned long)(void *)&st);
    tracks = (int)st.dwReturn;
    sk.dwCallback = 0;
    sk.dwTo = (unsigned)track;
    r = mciSendCommand(dev, 0x807, 0xaL, (unsigned long)(void *)&sk);
    if (r != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        mm_error_report(&g_error_log, "Failed to play requested CD Track", (long)(int)r);
        return 0;
    }
    if ((unsigned long)track == tracks)
        flags = 5L;
    else
        flags = 0xdL;
    pp.dwFrom = 0;
    pp.dwTo = 0;
    pp.dwFrom = (unsigned)track;
    pp.dwTo = (unsigned)(unsigned char)(track + 1);
    pp.dwCallback = g_cd_notify_hwnd;
    r = mciSendCommand(dev, 0x806, flags, (unsigned long)(void *)&pp);
    if (r != 0) {
        mciSendCommand(dev, 0x804, 0L, 0L);
        mm_error_report(&g_error_log, "Failed to play requested CD Track", (long)(int)r);
        return 0;
    }
    return 1;
}
