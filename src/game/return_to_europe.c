/* return_to_europe -- shipped at 1048:a37e.
 *
 * Ported by tools/port.py from matched/game/return_to_europe.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ask_text_resource(char *name);

#include "gameflag.h"


/* same object: 4-byte `push cs; call ` (rule 47) */

extern void unit_stack_arrives(void);
extern void viceroy_unit_msg5(col_i16 u);

void return_to_europe(void)
{
    if (g_game_flags & GAMEFLAG_INDEPENDENCE) {
        ask_text_resource("EUROPENOTLEAVE");
    } else {
        unit_stack_arrives();
        viceroy_unit_msg5(g_selected_unit);
    }
}
