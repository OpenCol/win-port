/* MSGetTextHeight: candidate for 1078:139b (115 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The font's height: select it into the screen DC, GetTextMetrics, restore, and return
 * tmHeight.
 *
 * RENAMED 2026-09-26 from font_metrics: it returns one metric, tmHeight, and its ten callers all store it as a height (re_namecheck receiver-agreement). Not font_height: that name is the per-module accessor family for GameFont->height, a record field, where this measures the Windows font through GetTextMetrics.
 *
 * RENAMED 2026-09-27 from measure_font_height: NOT PROPOSED BY mac_bind (it proposed Font_ctor_1008_d9c4, the ctor that calls this). Mac MSGetTextHeight(MSFontInfo**). Layout: Windows 1078 MSGetTypeFont, table_entry_handle, MSReleaseFont, 139b, font_line_height, text_extent; the Mac MS file has MSGetTypeFont, MSReleaseFont, MSGetTextHeight, MSGetTextLength, MSGetLineHeight. Callers: GRPort_String and GRPort_String_curfont are the Mac GRPort::String x2. The GameFont ctors carry the KingRoom (1008) and Popup (1050) uses. Arity 1. Font wrappers pass the font record's handle at +0, the shape of MSEnableItem_1048_e2dd (the menu handle at +0). They take MSName_<seg>_<off>, and the 1078 body takes the bare Mac name.
 */

#include "colonization.h"

#include "records.h"

typedef struct { int tmHeight, tmAscent, tmDescent, tmInternalLeading, tmExternalLeading, tmAveCharWidth, tmMaxCharWidth, tmWeight; unsigned char tmItalic, tmUnderlined, tmStruckOut, tmFirstChar, tmLastChar, tmDefaultChar, tmBreakChar, tmPitchAndFamily, tmCharSet; int tmOverhang, tmDigitizedAspectX, tmDigitizedAspectY; } TEXTMETRIC;
extern int GetTextMetrics(unsigned hdc, TEXTMETRIC *tm);

int MSGetTextHeight(unsigned font_handle)
{
    FontRec *font;
    unsigned hdc;
    unsigned old;
    TEXTMETRIC tm;

    font = (FontRec *)MSHLock(font_handle);
    hdc = GetDC(0);
    old = SelectObject(hdc, font->hfont);
    GetTextMetrics(hdc, &tm);
    SelectObject(hdc, old);
    ReleaseDC(0, hdc);
    MSHUnlock(font_handle);
    return tm.tmHeight;
}
