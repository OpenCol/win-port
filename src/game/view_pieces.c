/* view_pieces -- shipped at 1008:2d0d.
 *
 * Ported by tools/port.py from matched/game/view_pieces.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_view_mode;                          SEG20:0x7792
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void disable_menu_items_400_416(void);

/* 28 bytes at SEG20:0xa208 */



extern void redraw_map_area(col_i16 x, col_i16 y, col_i16 a, col_i16 b, col_i16 c);



void view_pieces(void *ctx)
{
    g_view_mode = 1;
    if (g_selected_unit >= 0)
        redraw_map_area(g_units[g_selected_unit].x, g_units[g_selected_unit].y, 1, 1, 1);
    disable_menu_items_400_416();
}
