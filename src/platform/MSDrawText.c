/* MSDrawText: candidate for 1070:ada4 (256 bytes).  Verdict: RELOCATABLE.
 *
 * DrawText on the WinG surface's DC (the GrafRec's +2) in the given font, coloured from palette entry
 * DGROUP:0x2ada through the WinG DIB colour table; flags 0 centre the text, bit 1 right-aligns
 * it, anything else left-aligns, always word-wrapped (DT_WORDBREAK, 0x10). A null font is reported
 * ("Error: NULL font in MSDrawText").
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
typedef struct { unsigned char rgbBlue, rgbGreen, rgbRed, rgbReserved; } RGBQUAD;

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1070_dc82(void *ctx, char *msg);

extern unsigned WinGGetDIBColorTable(unsigned hdc, unsigned start, unsigned n, RGBQUAD *q);

void MSDrawText(GrafRec *graf, unsigned font_handle, const char *s, void *r, int flags)
{
    FontRec *f;
    unsigned old;
    RGBQUAD q;
    unsigned fmt;

    if (font_handle == 0) {
        report_error_1070_dc82(&g_error_log, "Error: NULL font in MSDrawText");
        return;
    }
    f = (FontRec *)MSHLock(font_handle);
    old = SelectObject(graf->hdc, f->hfont);
    WinGGetDIBColorTable(graf->hdc, g_line_color_index, 1, &q);
    SetTextColor(graf->hdc, (unsigned)(q.rgbRed | (q.rgbGreen << 8)) | ((unsigned long)q.rgbBlue << 16));
    fmt = 0x10;
    if (flags == 0)
        fmt |= 1;
    else if (flags & 2)
        fmt |= 2;
    DrawText(graf->hdc, s, STRLEN(s), (RECT *)r, fmt);
    SelectObject(graf->hdc, old);
    MSHUnlock(font_handle);
}
