/* copy_screen_to_wingdc: candidate for 1070:ca01 (239 bytes).  Verdict: RELOCATABLE.
 *
 * Copies the screen rectangle r to the origin of the WinG surface's DC (GrafRec +2) by way of a compatible bitmap on
 * a fresh "DISPLAY" DC; each failed BitBlt is reported.
 *
 * RENAMED 2026-09-26 from blit_screen_to_memdc: BitBlts screen rectangle r into a temporary compatible memory DC, then BitBlts that memory DC into the window's DC at +2. Its own error strings say 'copy screen to memDC' and 'copy memDC to WinGDC'. The memory DC and bitmap are deleted before it returns. The name gives the throwaway intermediate as the destination; the real destination is the window's WinG DC.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

extern void report_error_1070_dc82(void *ctx, char *msg);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int rect_width(RECT *r);
extern int rect_height(RECT *r);

void copy_screen_to_wingdc(GrafRec *graf, RECT *r)
{
    unsigned hbm;
    int width;
    int height;
    unsigned screen;
    unsigned mem;

    width = rect_width(r);
    height = rect_height(r);
    screen = CreateDC("DISPLAY", 0, 0, 0);
    mem = CreateCompatibleDC(screen);
    hbm = CreateCompatibleBitmap(screen, width, height);
    SelectObject(mem, hbm);
    if (!BitBlt(mem, 0, 0, width, height, screen, r->left, r->top, 0xcc0020L))
        report_error_1070_dc82(&g_error_log, "BitBlt error: copy screen to memDC");
    if (!BitBlt(graf->hdc, 0, 0, width, height, mem, 0, 0, 0xcc0020L))
        report_error_1070_dc82(&g_error_log, "BitBlt error: copy memDC to WinGDC");
    DeleteDC(mem);
    DeleteDC(screen);
    DeleteObject(hbm);
}
