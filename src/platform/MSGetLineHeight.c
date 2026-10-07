/* MSGetLineHeight: candidate for 1078:140e (118 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The line height of a font record (handle to a locked record whose first word is the HFONT):
 * tmHeight + tmExternalLeading measured on the screen DC.
 *
 * RENAMED 2026-09-27 from font_line_height: Mac MSGetLineHeight(MSFontInfo**). Callers: ctl_line_height (the Popup wrapper) and ask_choice. Mac MSGetLineHeight's callers are 6 Popup methods, which are the wrapper's 6 callers exactly, plus MSCalcTextRect. Layout: in the font block after MSGetTextHeight (see 139b). Arity 1. Font wrappers pass the font record's handle at +0, the shape of MSEnableItem_1048_e2dd (the menu handle at +0). They take MSName_<seg>_<off>, and the 1078 body takes the bare Mac name.
 */

#include "colonization.h"

typedef struct { int tmHeight, tmAscent, tmDescent, tmInternalLeading, tmExternalLeading, tmAveCharWidth, tmMaxCharWidth, tmWeight; unsigned char tmItalic, tmUnderlined, tmStruckOut, tmFirstChar, tmLastChar, tmDefaultChar, tmBreakChar, tmPitchAndFamily, tmCharSet; int tmOverhang, tmDigitizedAspectX, tmDigitizedAspectY; } TEXTMETRIC;
#include "records.h"

extern int GetTextMetrics(unsigned hdc, TEXTMETRIC *tm);

int MSGetLineHeight(unsigned h)
{
    FontRec *font;
    unsigned old;
    TEXTMETRIC tm;
    unsigned dc;

    font = (FontRec *)MSHLock(h);
    dc = GetDC(0);
    old = SelectObject(dc, font->hfont);
    GetTextMetrics(dc, &tm);
    SelectObject(dc, old);
    ReleaseDC(0, dc);
    MSHUnlock(h);
    return tm.tmHeight + tm.tmExternalLeading;
}
