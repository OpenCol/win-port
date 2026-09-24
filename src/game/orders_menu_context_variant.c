/* orders_menu_context_variant -- shipped at 1048:aa10.
 *
 * Ported by tools/port.py from matched/game/orders_menu_context_variant.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern int far g_view_mode;                          SEG20:0x7792
 *   extern unsigned char far g_order_flags[];            SEG30:0x0050
 *
 * The file's own Win, Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void menu_call_da9f(char *m, col_i32 id, char *text);
void menu_enable_item_1048_e2dd(char *m, col_u32 id, char enable);
void orders_menu_init(Win *w);

#include "terrain.h"
#include "jobs.h"
#include "units.h"
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern col_i16 unit_colonist_class(col_i16 u);
extern void unit_class_flags(col_i16 u, col_i16 *is_colonist, col_i16 *is_type2);
extern col_i16 colony_at(col_i16 x, col_i16 y);
extern col_i16 square_european_nation(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` (rule 47) */




void orders_menu_context_variant(Win *w)
{
    col_i16 is_col;
    col_i16 is_t2;
    col_i16 y;
    unsigned char terr;
    register col_i16 u;
    register col_i16 x;

    u = g_selected_unit;
    if (g_view_mode == 0) {
        orders_menu_init(w);
        x = g_units[u].x;
        y = g_units[u].y;
        terr = terrain_at(x, y);
        is_col = 0;
        is_t2 = 0;
        if (unit_colonist_class(u) >= 0)
            unit_class_flags(u, &is_col, &is_t2);
        g_order_flags[10] = 0;
        if (is_col == 0) {
            menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 405L, 0);
            g_order_flags[5] = 0;
        } else if (colony_at(x, y) >= 0) {
            menu_call_da9f(((char *)((unsigned char *)(w) + 0x8c)), 405L, "Join Colony\tB");
        } else if ((col_i16)g_units[u].spec == JOB_CONVERT) {
            menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 405L, 0);
            g_order_flags[5] = 0;
        }
        if (is_t2 == 0) {
            menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 406L, 0);
            g_order_flags[6] = 0;
            menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 407L, 0);
            g_order_flags[7] = 0;
        }
        if ((terr >= TERRAIN_FOREST_FIRST && terr < TERRAIN_FOREST_2_FIRST) || (terr >= TERRAIN_FOREST_2_FIRST && terr < TERRAIN_ARCTIC))
            menu_call_da9f(((char *)((unsigned char *)(w) + 0x8c)), 406L, "Clear Forest\tP");
        else {
        }
        if (terr == TERRAIN_MOUNTAINS || terr == TERRAIN_HILLS) {
            menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 406L, 0);
            g_order_flags[6] = 0;
        }
        if (g_units[g_selected_unit].type >= UNIT_SHIP_FIRST && g_units[g_selected_unit].type <= UNIT_SHIP_LAST) {
            if (terr != TERRAIN_SEA_LANE) {
                menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 413L, 0);
                g_order_flags[13] = 0;
            }
        } else {
            menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 413L, 0);
            g_order_flags[13] = 0;
            menu_call_da9f(((char *)((unsigned char *)(w) + 0x8c)), 411L, "Go To Place\tG");
        }
        if (g_unit_types[g_units[g_selected_unit].type].holds > 0) {
            if (square_european_nation(x, y) < 0) {
                menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 408L, 0);
                g_order_flags[8] = 0;
                menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 409L, 0);
                g_order_flags[9] = 0;
            }
            if (g_units[g_selected_unit].cargo_count == 0) {
                menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 415L, 0);
                g_order_flags[15] = 0;
            }
        } else {
            menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 408L, 0);
            g_order_flags[8] = 0;
            menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 409L, 0);
            g_order_flags[9] = 0;
            menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 412L, 0);
            g_order_flags[12] = 0;
            menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), 415L, 0);
            g_order_flags[15] = 0;
        }
    }
}
