/* ctable_hpal -- shipped at 1070:97d4.
 *
 * Ported by tools/port.py from matched/platform/ctable_hpal.c.
 *
 * The file's own CTable dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_u16 ctable_hpal(CTable *t)
{
    return t->hpal;
}
