/* MSGetTypeFont -- shipped at 1078:11c6.
 *
 * Ported by tools/port.py from matched/platform/MSGetTypeFont.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own FontRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal CreateFontIndirect(LOGFONT *lf);
#endif

extern col_u16 MSAllocateHandle(col_u32 size);
extern FontRec * MSHLock(col_i16 h);
extern void MSHUnlock(col_i16 h);

col_u16 MSGetTypeFont(col_i16 kind, col_i16 height)
{
    FontRec *f;
    LOGFONT lf;
    col_u16 h;
    col_u16 hfont;

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
        strcpy(lf.lfFaceName, "Times New Roman");
        break;
    case 1:
        lf.lfPitchAndFamily |= 0x20;
        strcpy(lf.lfFaceName, "Arial");
        break;
    case 2:
        lf.lfPitchAndFamily |= 0;
        strcpy(lf.lfFaceName, "System");
        break;
    }
    h = 0;
    hfont = CreateFontIndirect(&lf);
    if (hfont != 0) {
        h = MSAllocateHandle(4L);
        f = MSHLock(h);
        f->hfont = hfont;
        f->owned = 1;
        MSHUnlock(h);
    }
    return h;
}
