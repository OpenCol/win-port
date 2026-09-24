/* tutorial_colony_food -- shipped at 1010:4251.
 *
 * Ported by tools/port.py from matched/game/tutorial_colony_food.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_panel_toggle;                       DGROUP:0x0c0c
 *   extern char far g_colony_window;                     SEG20:0x25be
 *   extern char far g_colony_win_rec;                    SEG20:0x25e4
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern unsigned far g_tutorial_flags;                SEG20:0x7782
 *   extern int far g_over_lo[];                          SEG21:0x018c
 *   extern int far g_over_band[];                        SEG21:0x01b4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void show_text_dialog_1010_e528(char *msg);
void win_disable_1010_e459(void *c);
void win_enable_1010_e4d4(void *c);

#include "gameflag.h"
#include "goods.h"







extern void colony_refresh_all(void);
extern void colony_draw_colonists(void *c, col_i16 a);
extern void colony_draw_buildings(void *c, col_i16 a);
extern void colony_draw_work_area(void *c, col_i16 a);
extern void empty_1010_b03a(void *c, col_i16 a, col_i16 b);
extern void colony_draw_panel6(void *c, col_i16 a);
extern void colony_draw_carriers(void *c, col_i16 a);
extern void colony_draw_warehouse(void *c, col_i16 a);
extern void colony_draw_status(void *c);
/* same object: 4-byte `push cs; call ` (rule 47) */



void tutorial_colony_food(void)
{
    g_panel_toggle = 1;
    colony_refresh_all();
    colony_draw_colonists(&(*g_colony_window), 1);
    colony_draw_buildings(&(*g_colony_window), 1);
    colony_draw_work_area(&(*g_colony_window), 1);
    empty_1010_b03a(&(*g_colony_window), 1, 0);
    colony_draw_panel6(&(*g_colony_window), 1);
    colony_draw_carriers(&(*g_colony_window), 1);
    colony_draw_warehouse(&(*g_colony_window), 1);
    colony_draw_status(&(*g_colony_window));
    if ((g_game_flags & GAMEFLAG_TUTORIAL) && !(g_tutorial_flags & 0x10) && (g_over_lo[GOODS_FOOD] != 0 || g_over_band[GOODS_FOOD] != 0)) {
        win_disable_1010_e459(&g_colony_win_rec);
        show_text_dialog_1010_e528("TUTORIAL16");
        win_enable_1010_e4d4(&g_colony_win_rec);
        g_tutorial_flags |= 0x10;
    }
}
