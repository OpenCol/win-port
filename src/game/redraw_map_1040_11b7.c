/* redraw_map_1040_11b7 -- shipped at 1040:11b7.
 *
 * Ported by tools/port.py from matched/game/redraw_map_1040_11b7.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern int far g_view_nation;                        SEG20:0x7798
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_all_to_screen_1040_07d5(void);
void draw_colonies_view(void);
void draw_settlements_view(void);
void draw_units_view(void);
void map_copy_backdrop_1040_07b9(void);

extern void redraw_map_view(col_i16 a);







void redraw_map_1040_11b7(col_i16 flag)
{
    redraw_map_view(g_reveal_all != 0 ? -1 : g_view_nation);
    draw_settlements_view();
    draw_colonies_view();
    map_copy_backdrop_1040_07b9();
    draw_units_view();
    if (flag != 0)
        copy_all_to_screen_1040_07d5();
}
