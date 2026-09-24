/* alloc_slot -- shipped at 1068:b75a.
 *
 * Ported by tools/port.py from matched/gfx/alloc_slot.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char g_slot_used[];                  DGROUP:0x56a0
 *   extern long g_slot_state[];                          DGROUP:0x56a4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void select_slot(col_i16 n);

/* same object: 4-byte `push cs; call ` */


col_i16 alloc_slot(void)
{
    register col_i16 i;
    register col_i16 found;

    found = -1;
    for (i = 0; found < 0 && i < 4; i++) {
        if (g_slot_used[i] == 0) {
            g_slot_used[i] = 1;
            g_slot_state[i] = 0L;
            found = i;
        }
    }
    select_slot(found);
    return found;
}
