/* MSGetCDInfo: candidate for 1078:0fd7 (222 bytes).  Verdict: RELOCATABLE.
 *
 * Current position of the open CD in TMSF: the time format is set, MCI_STATUS_POSITION queried, and the
 * minute, second and track bytes stored through whichever out-pointers are given. 0 with
 * no device or on any failure, else 1.
 */

#include "colonization.h"

#include "w16mci.h"
#define MCI_TMSF_TRACK(t)  ((unsigned char)(t))
#define MCI_TMSF_MINUTE(t) ((unsigned char)(((unsigned)(t)) >> 8))
#define MCI_TMSF_SECOND(t) ((unsigned char)((t) >> 16))
   /* 16 bytes in this build */

int MSGetCDInfo(int *minutes, int *seconds, int *tracks)
{
    unsigned long flags;
    unsigned long r;
    MCI_STATUS_PARMS st;
    MCI_SET_PARMS sp;
    unsigned dev;

    if (g_cd_device == 0)
        return 0;
    else
        dev = g_cd_device;
    sp.dwTimeFormat = 10L;
    flags = 0x400L;
    r = mciSendCommand(dev, 0x80d, flags, (unsigned long)(void *)&sp);
    if (r != 0)
        return 0;
    flags = 0x100L;
    st.dwItem = 2L;
    r = mciSendCommand(dev, 0x814, flags, (unsigned long)(void *)&st);
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
