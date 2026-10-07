/* enumerate_font_families: candidate for 1078:1639 (124 bytes).  Verdict: RELOCATABLE.
 *
 * EnumFontFamilies over the screen DC with enum_font_family_proc as the callback and the font
 * record as its lParam; the record's count at +0x100 is cleared first and returned.
 */

#include "colonization.h"

#include "records.h"
typedef int (*FARPROC)();

int enumerate_font_families(FontFile *lparam)
{
    unsigned hdc;
    FARPROC proc;

    hdc = GetDC(0);
    lparam->count = 0;
    proc = MakeProcInstance((FARPROC)enum_font_family_proc, g_hinstance);
    EnumFontFamilies(hdc, 0, proc, lparam);
    FreeProcInstance(proc);
    ReleaseDC(0, hdc);
    return lparam->count;
}
