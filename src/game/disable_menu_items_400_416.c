/* disable_menu_items_400_416 -- shipped at 1008:2cdc.
 *
 * Ported by tools/port.py from matched/game/disable_menu_items_400_416.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_menu_owner[];                      SEG20:0x0656
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void menu_enable_item_1008_cec2(void *w, col_i32 id, col_i16 enable);

/* same object: 4-byte `push cs; call ` */


void disable_menu_items_400_416(void)
{
    register col_i16 i;

    for (i = 0x190; i <= 0x1a0; i++)
        menu_enable_item_1008_cec2((&g_menu_owner), (col_i32)i, 0);
}
