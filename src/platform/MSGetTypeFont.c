/* MSGetTypeFont: candidate for 1078:11c6 (257 bytes).  Verdict: RELOCATABLE.
 *
 * Creates one of three typefaces (Times New Roman, Arial, System: kinds 0..2, the
 * first two with pitch-and-family bits 0x10 / 0x20) at the given height (negative, i.e.
 * character height) and wraps the HFONT in a 4-byte owned font record; 0 when GDI refuses.
 * The shipped code clears lfFaceName[32] -- one past the 32-byte field, into the record
 * pointer's low byte -- and that is reproduced.
 */

#include "colonization.h"

#include "w16types.h"

#include "records.h"

unsigned MSGetTypeFont(int kind, int height)
{
    FontRec *font;
    LOGFONT lf;
    unsigned h;
    unsigned hfont;

    lf.lfHeight = -height;
    lf.lfWidth = 0;
    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    lf.lfWeight = 0;
    lf.lfItalic = 0;
    lf.lfUnderline = 0;
    lf.lfStrikeOut = 0;
    lf.lfCharSet = 1;
    lf.lfOutPrecision = 4;
    lf.lfClipPrecision = 1;
    lf.lfQuality = 0;
    lf.lfPitchAndFamily = 0;
    lf.lfFaceName[32] = 0;
    switch (kind) {
    case 0:
        lf.lfPitchAndFamily |= 0x10;
        STRCPY(lf.lfFaceName, "Times New Roman");
        break;
    case 1:
        lf.lfPitchAndFamily |= 0x20;
        STRCPY(lf.lfFaceName, "Arial");
        break;
    case 2:
        lf.lfPitchAndFamily |= 0;
        STRCPY(lf.lfFaceName, "System");
        break;
    }
    h = 0;
    hfont = CreateFontIndirect(&lf);
    if (hfont != 0) {
        h = MSAllocateHandle(4L);
        font = (FontRec *)MSHLock(h);
        font->hfont = hfont;
        font->owned = 1;
        MSHUnlock(h);
    }
    return h;
}
