/* PORT: this file's own declarations of font_handle_1070_9806 removed: the header declares it from the definition (W1 step 3). */
/* font_row_rect: candidate for 1070:2ea3 (192 bytes).  Verdict: RELOCATABLE.
 * symbol: font_row_rect
 *
 * A list row rectangle at (x, y): `w` wide and one default-font row high (tmHeight of the
 * font at DGROUP:0x2ad2 measured on a memory DC, at least 20). The module's font lookup is defined here because the shipped call is
 * the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
typedef struct { int tmHeight, tmAscent, tmDescent, tmInternalLeading, tmExternalLeading, tmAveCharWidth, tmMaxCharWidth, tmWeight; unsigned char tmItalic, tmUnderlined, tmStruckOut, tmFirstChar, tmLastChar, tmDefaultChar, tmBreakChar, tmPitchAndFamily, tmCharSet; int tmOverhang, tmDigitizedAspectX, tmDigitizedAspectY; } TEXTMETRIC;

#include "records.h"

extern int GetTextMetrics(unsigned hdc, TEXTMETRIC *tm);


void font_row_rect(int x, int y, int w, RECT *r)
{
    int h;
    unsigned dc;
    FontRec *font;
    unsigned dc0;
    TEXTMETRIC tm;

    dc0 = GetDC(0);
    dc = CreateCompatibleDC(dc0);
    ReleaseDC(0, dc0);
    font = (FontRec *)MSHLock(font_handle_1070_9806((Font *)g_default_font_ref));
    SelectObject(dc, font->hfont);
    MSHUnlock(font_handle_1070_9806((Font *)g_default_font_ref));
    GetTextMetrics(dc, &tm);
    h = tm.tmHeight;
    DeleteDC(dc);
    if (h < 20)
        h = 20;
    SetRect(r, x, y, x + w, y + h);
}
