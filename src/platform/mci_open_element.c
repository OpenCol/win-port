/* PORT: mciSendCommand's parameter block is passed as uintptr_t, not unsigned long: 8 bytes on 64-bit Windows (W9). */
/* mci_open_element: candidate for 1078:2e1c (278 bytes).  Verdict: RELOCATABLE.
 *
 * Opens an animation element (a file name) as a child of the window (style 0x40000000, the
 * window's HWND as parent) through MCI_OPEN; on success a 12-byte record is allocated and
 * locked -- its own handle, the device id, the device's speed (status item 0x4001) and the
 * constant 0x20000 -- after the MCI window is shown (MCI_WINDOW, SW_SHOW); the record is
 * returned, null when the open failed.
 */

#include "colonization.h"

#include "records.h"

typedef struct { unsigned long dwCallback; unsigned wDeviceID; unsigned wReserved0; const char *lpstrDeviceType; const char *lpstrElementName; const char *lpstrAlias; unsigned long dwStyle; unsigned hWndParent; unsigned wReserved1; } MCI_ANIM_OPEN_PARMS;
typedef struct { unsigned long dwCallback; unsigned hWnd; unsigned wReserved1; unsigned nCmdShow; unsigned wReserved2; const char *lpstrText; } MCI_ANIM_WINDOW_PARMS;

AnimRec *mci_open_element(MSWin *win, const char *name)
{
    AnimRec *rec;
    MCI_ANIM_OPEN_PARMS op;
    MCI_ANIM_WINDOW_PARMS wp;
    MCI_DGV_STATUS_PARMS st;
    unsigned h;

    rec = 0;
    op.dwCallback = 0;
    op.wDeviceID = 0;
    op.lpstrDeviceType = 0;
    op.lpstrElementName = name;
    op.lpstrAlias = 0;
    op.dwStyle = 0x40000000L;
    op.hWndParent = win->hwnd;
    if (mciSendCommand(0, 0x803, 0x30200L, (uintptr_t)(void *)&op) == 0) {
        h = MSAllocateHandle(12L);
        rec = (AnimRec *)MSHLock(h);
        rec->handle = h;
        rec->dev = op.wDeviceID;
        wp.dwCallback = 0;
        wp.hWnd = 0;
        wp.nCmdShow = 5;
        wp.lpstrText = 0;
        mciSendCommand(rec->dev, 0x841, 0x40000L, (uintptr_t)(void *)&wp);
        st.dwItem = 0x4001;
        mciSendCommand(rec->dev, 0x814, 0x100L, (uintptr_t)(void *)&st);
        rec->speed = (unsigned)st.dwReturn;
        rec->rate = 0x20000L;
    }
    return rec;
}
