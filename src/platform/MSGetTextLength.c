/* MSGetTextLength: candidate for 1078:1484 (142 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Width of s in the font: select it into the screen DC, GetTextExtentPoint, restore.
 *
 * RENAMED 2026-09-27 from text_extent: Mac MSGetTextLength(MSFontInfo**, char*). Layout: the Windows font block after font_line_height; the Mac has MSGetTextLength between MSGetTextHeight and MSGetLineHeight, the same neighbourhood with two entries swapped. Arity 2 = (font_handle, s). FAMILY of 8 byte-identical wrappers of text_extent (1078:1484). The union of their callers is EXACTLY Mac MSGetTextLength's 28 callers: PediaWin Do*Article x4 (1060:6977); ColonyWin DrawColonists, DrawMarket, label_building, label_cargo and label_map (1010:e2d2); ParseFancyTextAndShow (1040:a8dd); PutLabel (1038:8713); TeepeeDraw (1040:44a9); europe_label_cargo (1048:dc4e); ReportWin ReportsEcon, ReportsForeign, ReportsIndian, ReportsLabor, Score, TradeRoutes and StatWin_Stats (1058:be29); Popup StringPourCalculate, StringPourDraw, button_x_size, draw, edit, item, make_controls and sprite (1050:8391). Arity 2 = (font, s). mac_bind proposed only 1058:be29. Font wrappers pass the font record's handle at +0, the shape of MSEnableItem_1048_e2dd (the menu handle at +0). They take MSName_<seg>_<off>, and the 1078 body takes the bare Mac name.
 */

#include "colonization.h"

#include "records.h"

#include "w16types.h"   /* SIZE, WINDOWS.H tagSIZE */
extern int GetTextExtentPoint(unsigned hdc, const char *s, int n, SIZE *sz);

int MSGetTextLength(unsigned font_handle, const char *s)
{
    FontRec *font;
    unsigned hdc;
    unsigned old;
    int w;
    Size sz;

    font = (FontRec *)MSHLock(font_handle);
    hdc = GetDC(0);
    old = SelectObject(hdc, font->hfont);
    GetTextExtentPoint(hdc, s, STRLEN(s), (SIZE *)&sz);
    w = sz.cx;
    SelectObject(hdc, old);
    ReleaseDC(0, hdc);
    MSHUnlock(font_handle);
    return w;
}
