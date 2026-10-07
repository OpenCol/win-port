/* CTable_set_key_and_reinstall: candidate for 1068:097e (69 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_set_key_and_reinstall
 *
 * Records a key colour (three bytes at +0x414) and re-derives the table.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void CTable_SetScale(CTable *table, short first, short count, short mode);
void CTable_set_key_and_reinstall(CTable *table, short first, short count, short mode, char r, char g, char b)
{
    table->r = r;
    table->g = g;
    table->b = b;
    CTable_SetScale(table, first, count, mode);
}
