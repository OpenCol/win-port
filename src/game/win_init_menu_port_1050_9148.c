/* win_init_menu_port_1050_9148 -- shipped at 1050:9148.
 *
 * Ported by tools/port.py from matched/game/win_init_menu_port_1050_9148.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void GRPort_Init_wh_1050_91ea(Win *o);
void menu_attach_1050_906a(void *sub);

/* same object: 4-byte `push cs; call ` */



void win_init_menu_port_1050_9148(Win *o)
{
    menu_attach_1050_906a(((char *)((unsigned char *)(o) + 0x26)));
    GRPort_Init_wh_1050_91ea(o);
}
