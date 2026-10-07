/* MSGetTextLength_1010_e2d2: candidate for 1010:e2d2 (38 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * MSGetTextLength with the font handle at +0 of the record and two arguments.
 *
 * TRANSFERRED: byte-identical to `MSGetTextLength_1048_dc4e` (1048:dc4e) and reaching the same routine, one of
 * 10 copies of this function in the image.
 *
 * NOT THREE SCALARS. The last two words are one `const char *`: `MSGetTextLength`'s own
 * reconstruction takes `(unsigned hfont, const char *s)` and passes the pointer to
 * `GetTextExtentPoint`, and `Popup_StringPourDraw` pushes a real string through this
 * forwarder as a segment and an offset. A forwarder that passes everything through can
 * never show the split itself (§119).
 *
 * RENAMED 2026-09-27 from font_text_extent_1010_e2d2: Wrapper of MSGetTextLength (1078:1484). FAMILY of 8 byte-identical wrappers of text_extent (1078:1484). The union of their callers is EXACTLY Mac MSGetTextLength's 28 callers: PediaWin Do*Article x4 (1060:6977); ColonyWin DrawColonists, DrawMarket, label_building, label_cargo and label_map (1010:e2d2); ParseFancyTextAndShow (1040:a8dd); PutLabel (1038:8713); TeepeeDraw (1040:44a9); europe_label_cargo (1048:dc4e); ReportWin ReportsEcon, ReportsForeign, ReportsIndian, ReportsLabor, Score, TradeRoutes and StatWin_Stats (1058:be29); Popup StringPourCalculate, StringPourDraw, button_x_size, draw, edit, item, make_controls and sprite (1050:8391). Arity 2 = (font, s). mac_bind proposed only 1058:be29. Font wrappers pass the font record's handle at +0, the shape of MSEnableItem_1048_e2dd (the menu handle at +0). They take MSName_<seg>_<off>, and the 1078 body takes the bare Mac name.
 */

#include "colonization.h"

#include "classes.h"
extern int MSGetTextLength(unsigned hfont, const char *s);

int MSGetTextLength_1010_e2d2(Font *font, const char *s)
{
    return MSGetTextLength(font->hfont, s);
}
