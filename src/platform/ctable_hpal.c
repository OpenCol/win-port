/* ctable_hpal: candidate for 1070:97d4 (26 bytes).  Verdict: EXACT.
 *
 * A colour table's GDI palette handle (+0x404).
 */

#include "colonization.h"

#include "classes.h"

unsigned ctable_hpal(CTable *table)
{
    return table->hpal;
}
