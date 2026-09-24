/* set_current_slot_state -- shipped at 1068:b7a9.
 *
 * Ported by tools/port.py from matched/gfx/set_current_slot_state.c.
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
void select_slot(col_i16 n);

/* same object: 4-byte `push cs; call ` */


void set_current_slot_state(col_i32 state)
{
    register col_i16 cur;

    cur = g_current_slot;
    if (cur >= 0) {
        select_slot(-1);
        g_slot_state[cur] = state;
        select_slot(cur);
    }
}
