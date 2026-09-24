/* select_slot -- shipped at 1068:b70c.
 *
 * Ported by tools/port.py from matched/gfx/select_slot.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_current_slot;                           DGROUP:0x3246
 *   extern long g_slot_state[];                          DGROUP:0x56a4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i32 random_get_seed(void);
void random_set_seed(col_i32 state);

/* same object: 4-byte `push cs; call ` (rule 47) */

void select_slot(col_i16 n)
{
    col_i16 i = n;

    if (g_current_slot == i)
        goto done;
    if (g_current_slot >= 0)
        g_slot_state[g_current_slot] = random_get_seed();
    g_current_slot = i;
    if (i >= 0)
        random_set_seed(g_slot_state[i]);
done:
    ;
}
