/* MSDrawString: candidate for 1070:aa95 (519 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws a string at a point with an alignment mask: the text rectangle (extent x metrics)
 * is centred on (x, y) and then shifted by half its size for bit 1 (left edge at x), 2
 * (right edge), 4 (top edge at y) or 8 (bottom edge); it is clipped to `clip`, drawn with
 * TA_UPDATECP alignment from the moved-to corner in the line colour, and the DC's text
 * alignment and font restored. A null font is reported ("Error: NULL font in MSDrawString").
 *
 * RENAMED 2026-09-27 from MSDrawString_aligned: the Mac MSDrawString(CGrafPort*, MSFontInfo**, char*, short, short, Rect*, short) is 20 bytes with a handle as 2, and this function cleans 20 at its 4 call sites; the Mac's only callers are the two GRPort::String overloads, and this function's are GRPort_String and GRPort_String_curfont. Its own error text already says MSDrawString.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
typedef struct { unsigned char rgbBlue, rgbGreen, rgbRed, rgbReserved; } RGBQUAD;

extern void report_error_1070_dc82(void *ctx, char *msg);

extern int MSGetTextLength(unsigned hfont, const char *s);
extern int MSGetTextHeight(unsigned hfont);
extern int rect_height(RECT *r);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int rect_width(RECT *r);

extern unsigned WinGGetDIBColorTable(unsigned hdc, unsigned start, unsigned n, RGBQUAD *q);

void MSDrawString(GrafRec *graf, unsigned hfont, const char *s, int x, int y, RECT *clip, int align)
{
    FontRec *f;
    RECT r;
    unsigned oldalign;
    int halfw;
    int halfh;
    unsigned old;
    RGBQUAD q;

    if (hfont == 0) {
        report_error_1070_dc82(&g_error_log, "Error: NULL font in MSDrawString");
        return;
    }
    f = (FontRec *)MSHLock(hfont);
    SetRect(&r, x, y, x + MSGetTextLength(hfont, s), y + MSGetTextHeight(hfont));
    old = SelectObject(graf->hdc, f->hfont);
    WinGGetDIBColorTable(graf->hdc, g_line_color_index, 1, &q);
    SetTextColor(graf->hdc, (unsigned)(q.rgbRed | (q.rgbGreen << 8)) | ((unsigned long)q.rgbBlue << 16));
    halfh = rect_height(&r) / 2;
    halfw = rect_width(&r) / 2;
    OffsetRect(&r, -halfw, -halfh);
    if (align & 1)
        OffsetRect(&r, halfw, 0);
    if (align & 4)
        OffsetRect(&r, 0, halfh);
    if (align & 8)
        OffsetRect(&r, 0, -halfh);
    if (align & 2)
        OffsetRect(&r, -halfw, 0);
    oldalign = GetTextAlign(graf->hdc);
    SetTextAlign(graf->hdc, 1);
    MoveTo(graf->hdc, r.left, r.top);
    IntersectRect(&r, &r, clip);
    DrawText(graf->hdc, s, STRLEN(s), &r, 0);
    SetTextAlign(graf->hdc, oldalign);
    SelectObject(graf->hdc, old);
    MSHUnlock(hfont);
}
