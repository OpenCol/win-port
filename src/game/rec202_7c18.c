/* rec202_7c18 -- shipped at 1010:734b.
 *
 * Ported by tools/port.py from matched/game/rec202_7c18.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_colony_window;                     SEG20:0x25be
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void colony_choose_build(void *w, col_i16 a);

/* same object: 4-byte `push cs; call ` */


void rec202_7c18(col_i16 a)
{
    colony_choose_build(&(*g_colony_window), a);
}
