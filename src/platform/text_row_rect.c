/* text_row_rect: candidate for 1070:2dad (246 bytes).  Verdict: RELOCATABLE.
 * symbol: text_row_rect
 *
 * The rectangle a list row of text `s` takes at (x, y): its extent in the default font
 * (DGROUP:0x2ad2) measured on a memory DC plus 37, by the font's height (at least 20). The
 * module's font lookup is defined here because the shipped call is the compiler's own
 * `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
typedef struct { int tmHeight, tmAscent, tmDescent, tmInternalLeading, tmExternalLeading, tmAveCharWidth, tmMaxCharWidth, tmWeight; unsigned char tmItalic, tmUnderlined, tmStruckOut, tmFirstChar, tmLastChar, tmDefaultChar, tmBreakChar, tmPitchAndFamily, tmCharSet; int tmOverhang, tmDigitizedAspectX, tmDigitizedAspectY; } TEXTMETRIC;
#include "records.h"

extern int GetTextMetrics(unsigned hdc, TEXTMETRIC *tm);
extern int GetTextExtentPoint(unsigned hdc, const char *s, int n, SIZE *sz);

extern unsigned font_handle_1070_9806(void *ref);

void text_row_rect(char *s, int x, int y, RECT *r)
{
    unsigned dc;
    int h;
    int w;
    unsigned old;
    FontRec *font;
    unsigned dc0;
    SIZE sz;
    TEXTMETRIC tm;

    dc0 = GetDC(0);
    dc = CreateCompatibleDC(dc0);
    ReleaseDC(0, dc0);
    font = (FontRec *)MSHLock(font_handle_1070_9806((Font *)g_default_font_ref));
    old = SelectObject(dc, font->hfont);
    MSHUnlock(font_handle_1070_9806((Font *)g_default_font_ref));
    GetTextMetrics(dc, &tm);
    h = tm.tmHeight;
    GetTextExtentPoint(dc, s, STRLEN(s), &sz);
    w = sz.cx;
    SelectObject(dc, old);
    DeleteDC(dc);
    w += 37;
    if (h < 20)
        h = 20;
    SetRect(r, x, y, x + w, y + h);
}
