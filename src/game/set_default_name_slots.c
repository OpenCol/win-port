/* set_default_name_slots -- shipped at 1008:cc49.
 *
 * Ported by tools/port.py from matched/game/set_default_name_slots.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_name_default_0[];                      DGROUP:0x093f
 *   extern char g_name_default_1[];                      DGROUP:0x0949
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_name_slot_1008_cdca(col_i16 slot, char *s);

/* same object: 4-byte `push cs; call ` (rule 47) */


void set_default_name_slots(void)
{
    set_name_slot_1008_cdca(0, g_name_default_0);
    set_name_slot_1008_cdca(1, g_name_default_1);
}
