/* MMSYSTEM: MCI and sndPlaySound, Win16 on Win32 (roadmap W4).
 *
 * The parameter blocks the game passes are its own structures in the port's layout. Most
 * (PLAY, SEEK, SET, STATUS, PAUSE, RESUME, generic) are a callback and DWORDs and match
 * Win32's; three differ: OPEN (a reserved word after the device id), the animation WINDOW
 * block (reserved words between its fields) and WHERE/PUT (a 16-bit RECT). With MCI_NOTIFY
 * the callback is a window token. The CD-less fallback to music files is a separate item.
 */
#include <string.h>
#include "w16.h"
#include <mmsystem.h>

typedef struct {
    unsigned long dwCallback; unsigned wDeviceID, wReserved0; const char *lpstrDeviceType, *lpstrElementName,
        *lpstrAlias; unsigned long dwStyle; unsigned hWndParent, wReserved1;
} OPEN_G;
typedef struct { unsigned long dwCallback; unsigned hWnd, wReserved1, nCmdShow, wReserved2; const char *lpstrText; } WINDOW_G;
typedef struct { unsigned long dwCallback; RECT16 rc; } RECT_G;

unsigned long w16_mciSendCommand(unsigned dev, unsigned msg, unsigned long flags, unsigned long parms) W16(mciSendCommand);
unsigned long w16_mciSendCommand(unsigned dev, unsigned msg, unsigned long flags, unsigned long parms)
{
    void *p = (void *)(uintptr_t)parms;
    MCIERROR r;
    DWORD_PTR cb = 0;
    if (w16_lockstep())                 /* no device, as winsim's default answers (W6) */
        return 1;
    if (p && (flags & MCI_NOTIFY))
        cb = (DWORD_PTR)w16_handle((unsigned)(*(unsigned long *)p & 0xFFFF));
    switch (msg) {
    case MCI_OPEN: {
        OPEN_G *g = p;
        MCI_ANIM_OPEN_PARMSA o;
        memset(&o, 0, sizeof o);
        o.dwCallback = cb;
        o.lpstrDeviceType = g->lpstrDeviceType;
        o.lpstrElementName = g->lpstrElementName;
        o.lpstrAlias = g->lpstrAlias;
        o.dwStyle = g->dwStyle;
        o.hWndParent = (flags & MCI_ANIM_OPEN_PARENT) ? W16_HWND(g->hWndParent) : 0;
        r = mciSendCommandA(dev, msg, flags, (DWORD_PTR)&o);
        g->wDeviceID = o.wDeviceID;
        w16_trace("mci open %s %s = %lu (device %u)", g->lpstrDeviceType && (uintptr_t)g->lpstrDeviceType > 0xFFFF
                  ? g->lpstrDeviceType : "?", (flags & MCI_OPEN_ELEMENT) && g->lpstrElementName ? g->lpstrElementName : "",
                  (unsigned long)r, o.wDeviceID);
        return r;
    }
    case MCI_WINDOW: {
        WINDOW_G *g = p;
        MCI_ANIM_WINDOW_PARMSA w;
        memset(&w, 0, sizeof w);
        w.dwCallback = cb;
        w.hWnd = W16_HWND(g->hWnd);
        w.nCmdShow = g->nCmdShow;
        w.lpstrText = g->lpstrText;
        return mciSendCommandA(dev, msg, flags, (DWORD_PTR)&w);
    }
    case MCI_WHERE: case MCI_PUT: {
        RECT_G *g = p;
        MCI_ANIM_RECT_PARMS rp;
        memset(&rp, 0, sizeof rp);
        rp.dwCallback = cb;
        w16_rect_in(&rp.rc, &g->rc);
        r = mciSendCommandA(dev, msg, flags, (DWORD_PTR)&rp);
        if (msg == MCI_WHERE)
            w16_rect_out(&g->rc, &rp.rc);
        return r;
    }
    default:
        if (p && cb)
            *(unsigned long *)p = (unsigned long)cb;
        r = mciSendCommandA(dev, msg, flags, (DWORD_PTR)p);
        return r;
    }
}

unsigned w16_mciGetDeviceID(const char *name) W16(mciGetDeviceID);
unsigned w16_mciGetDeviceID(const char *name) { return mciGetDeviceIDA(name); }

int w16_sndPlaySound(const char *name, unsigned flags) W16(sndPlaySound);
int w16_sndPlaySound(const char *name, unsigned flags) { return w16_lockstep() ? 1 : sndPlaySoundA(name, flags); }
