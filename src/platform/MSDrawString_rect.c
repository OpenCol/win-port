/* MSDrawString_rect: candidate for 1070:ac9c (264 bytes).  Verdict: RELOCATABLE.
 *
 * MSDrawText's sibling: flags 0 draw centred, vertically centred, single-line (0x25);
 * otherwise the values 8 (DT_BOTTOM) and 2 (DT_RIGHT) are passed through. A null font is reported
 * ("Error: NULL font in MSDrawString").
 *
 * RENAMED 2026-09-27 from MSDrawString: the Mac MSDrawString(CGrafPort*, MSFontInfo**, char*, short, short, Rect*, short) is 20 bytes with a handle as 2; this function cleans 16 at its 4 call sites and has MSDrawText's width and callees, and its callers are the draw_string_curfont paths, not GRPort::String. The name belongs to 1070:aa95; this one keeps a descriptive suffix.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
typedef struct { unsigned char rgbBlue, rgbGreen, rgbRed, rgbReserved; } RGBQUAD;

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1070_dc82(void *ctx, char *msg);

extern unsigned WinGGetDIBColorTable(unsigned hdc, unsigned start, unsigned n, RGBQUAD *q);

void MSDrawString_rect(GrafRec *graf, unsigned font_handle, const char *s, void *r, int flags)
{
    FontRec *f;
    unsigned old;
    RGBQUAD q;
    unsigned fmt;

    if (font_handle == 0) {
        report_error_1070_dc82(&g_error_log, "Error: NULL font in MSDrawString");
        return;
    }
    f = (FontRec *)MSHLock(font_handle);
    old = SelectObject(graf->hdc, f->hfont);
    WinGGetDIBColorTable(graf->hdc, g_line_color_index, 1, &q);
    SetTextColor(graf->hdc, (unsigned)(q.rgbRed | (q.rgbGreen << 8)) | ((unsigned long)q.rgbBlue << 16));
    fmt = 0;
    if (flags == 0) {
        fmt = 0x25;
    } else {
        if (flags & 8)
            fmt |= 8;
        if (flags & 2)
            fmt |= 2;
    }
    DrawText(graf->hdc, s, STRLEN(s), (RECT *)r, fmt);
    SelectObject(graf->hdc, old);
    MSHUnlock(font_handle);
}
