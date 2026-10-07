/* MSInitGraf: candidate for 1070:a463 (468 bytes).  Verdict: RELOCATABLE.
 *
 * Creates a WinG off-screen surface for a rectangle: a WinG DC, the recommended DIB format
 * with the width rounded by round_up_4 and the height oriented by the setting at
 * DGROUP:0x39b4 (-1 bottom-up, 1 top-down, else WinG's recommendation scaled -- a bottom-up
 * recommendation is logged once), a 256-colour bitmap selected into the DC, and an 18-byte
 * record (handle, DC, bitmap, old bitmap, orientation, size, stride) in a global memory
 * block. 0 for an empty rectangle or when WinG refuses (reported).
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern int WinGRecommendDIBFormat(BITMAPINFO *bi);
extern unsigned WinGCreateBitmap(unsigned hdc, BITMAPINFO *bi, void *bits);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void report_error_1070_dc82(void *ctx, char *msg);

GrafRec *MSInitGraf(RECT *r)
{
    unsigned hmem;
    GrafRec *graf = 0;
    int w;
    unsigned hbm;
    BITMAPINFO bi;
    register int h;
    unsigned hdc;

    w = r->right - r->left;
    h = r->bottom - r->top;
    if (w <= 0 || h <= 0)
        return 0;
    if ((hdc = WinGCreateDC()) != 0) {
        WinGRecommendDIBFormat(&bi);
        bi.biWidth = round_up_4(w);
        if (bi.biHeight > 0) {
            if (g_wing_warned == 0)
                report_error_1070_dc82(&g_error_log, "WinG: Recommends bottom-up orientation");
            g_wing_warned = 1;
        }
        switch (g_wing_orientation) {
        case 1:
            bi.biHeight = -h;
            break;
        case -1:
            bi.biHeight = h;
            break;
        default:
            bi.biHeight = h * bi.biHeight;
            break;
        }
        memset_rtl(bi.colors, 0, 0x400);
        if ((hbm = WinGCreateBitmap(hdc, &bi, 0)) != 0) {
            hmem = MSAllocateHandle(0x12L);
            graf = (GrafRec *)GlobalLock(hmem);
            graf->handle = hmem;
            graf->hdc = hdc;
            graf->hobj = 0;
            graf->hbm = hbm;
            graf->stride = (int)bi.biWidth;
            graf->w = w;
            graf->h = h;
            graf->old = SelectObject(hdc, hbm);
            if (bi.biHeight < 0)
                graf->topdown = 1;
            else
                graf->topdown = 0;
            SetBkMode(hdc, 1);
        } else {
            report_error_1070_dc82(&g_error_log, "Error: Unable to create Bitmap in MSInitGraf");
            DeleteDC(hdc);
        }
    } else {
        report_error_1070_dc82(&g_error_log, "Error: Unable to create DC in MSInitGraf");
    }
    return graf;
}
