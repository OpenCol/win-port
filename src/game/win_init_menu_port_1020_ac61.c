/* win_init_menu_port_1020_ac61 -- shipped at 1020:ac61.
 *
 * Ported by tools/port.py from matched/game/win_init_menu_port_1020_ac61.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void GRPort_Init_wh_1020_af30(Win *o);
void menu_attach_1020_af04(void *sub);

/* same object: 4-byte `push cs; call ` */



void win_init_menu_port_1020_ac61(Win *o)
{
    menu_attach_1020_af04(((char *)((unsigned char *)(o) + 0x26)));
    GRPort_Init_wh_1020_af30(o);
}
