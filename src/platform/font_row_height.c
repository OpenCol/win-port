/* font_row_height: candidate for 1070:38c9 (173 bytes).  Verdict: RELOCATABLE.
 * symbol: font_row_height
 *
 * The default font's row height for lists: tmHeight of the font at DGROUP:0x2ad2 measured on a
 * memory DC, at least 20. The module's font lookup is defined here because the shipped call is
 * the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "classes.h"
typedef struct { int tmHeight, tmAscent, tmDescent, tmInternalLeading, tmExternalLeading, tmAveCharWidth, tmMaxCharWidth, tmWeight; unsigned char tmItalic, tmUnderlined, tmStruckOut, tmFirstChar, tmLastChar, tmDefaultChar, tmBreakChar, tmPitchAndFamily, tmCharSet; int tmOverhang, tmDigitizedAspectX, tmDigitizedAspectY; } TEXTMETRIC;
#include "records.h"

extern int GetTextMetrics(unsigned hdc, TEXTMETRIC *tm);

extern unsigned font_handle_1070_9806(void *ref);

int font_row_height(void)
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
    return h;
}
