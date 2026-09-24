/* colony_rebuild_screen -- shipped at 1018:8c35.
 *
 * Ported by tools/port.py from matched/game/colony_rebuild_screen.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a, g_stub_b, g_stub_c, g_stub_d;   
 *   extern int g_carrier_count;                          DGROUP:0x0c14
 *   extern int g_selected_carrier;                       DGROUP:0x0c16
 *   extern int g_c18;                                    DGROUP:0x0c18
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 colony_cargo_units(void);
void clear_marked_cells(void);
void colony_place_idle(void);
void colony_sort_colonists(void);
void init_grid_marks(void);

/* same object: 4-byte `push cs; call ` */






void colony_rebuild_screen(void)
{
    init_grid_marks();
    clear_marked_cells();
    colony_place_idle();
    colony_sort_colonists();
    g_carrier_count = colony_cargo_units();
    g_selected_carrier = 0;
    g_c18 = 0;
}
