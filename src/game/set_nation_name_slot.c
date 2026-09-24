/* set_nation_name_slot -- shipped at 1050:5ef7.
 *
 * Ported by tools/port.py from matched/game/set_nation_name_slot.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void say_nation_word(col_i16 k, col_i16 flag, char *s);
void set_name_slot_1050_8e3a(col_i16 i, char *s);

/* same object: 4-byte `push cs; call ` (rule 47) */


void set_nation_name_slot(col_i16 slot, col_i16 flag, col_i16 nation)
{
    char buf[0x50];

    buf[0] = 0;
    say_nation_word(nation, flag, buf);
    set_name_slot_1050_8e3a(slot, buf);
}
