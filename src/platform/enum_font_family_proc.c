/* enum_font_family_proc: candidate for 1078:15b1 (136 bytes).  Verdict: RELOCATABLE.
 *
 * The EnumFontFamilies callback: when the first three letters of the face name match the
 * table's wanted name (+0x102, compared lower-cased), a font is created from the LOGFONT and
 * stored in the table's handle array (+0x106) at its count (+0x100); always continues (1).
 */

#include "colonization.h"

#include "w16types.h"

int enum_font_family_proc(LOGFONT *lf, void *ntm, int type, long lparam)
{
    FontTable *tbl = (FontTable *)lparam;

    if (STRNCMP(STRLWR(lf->lfFaceName), STRLWR(tbl->name), 3) == 0)
        tbl->fonts[tbl->count++] = CreateFontIndirect(lf);
    return 1;
}
