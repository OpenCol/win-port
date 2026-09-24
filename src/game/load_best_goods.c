/* load_best_goods -- shipped at 1028:ba03.
 *
 * Ported by tools/port.py from matched/game/load_best_goods.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern unsigned char far g_goods_value[];            SEG30:0x0010
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern char far g_map_window[];                      SEG20:0x0658
 *
 * The file's own Unit, UnitType, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1028_e376(col_i16 a, col_i16 b);

#define NATION(u) ((unsigned char)((u).flags & 0xf))
/* 14 bytes at SEG20:0x7ac8 */
extern void viceroy_do_message(col_i16 a, col_i16 b);
extern col_i16 current_nation_boycotts(col_i16 bit);
extern void sort_int_pairs_by_key(col_i16 n, col_i16 *ids, col_i16 *keys);
extern void set_current_nation(col_i16 n);
extern void unit_add_cargo(col_i16 unit, col_i16 goods, col_i16 amount);
extern void ColChild_CancelMessage(void *w, col_i16 a);
extern void viceroy_message(col_i16 n);
extern void ColChild_AddNumberToMessage(void *w, col_i16 v);
extern void ColChild_AddToMessage(void *w, col_i16 word);
extern void viceroy_activate_message(col_i16 a, col_i32 b);

/* same object: 4-byte `push cs; call ` */


col_i16 load_best_goods(col_i16 u)
{
    col_i16 r;
    col_i16 space;
    col_i16 i;
    col_i16 qty;
    col_i16 best;
    col_i16 idx[16];
    col_i16 val[16];
    register col_i16 g;
    register col_i16 w;

    r = 1;
    best = -1;
    space = g_unit_types[g_units[u].type].holds - g_units[u].cargo_count;
    if (space < 1) {
        viceroy_do_message(0x15, 3);
        goto out;
    }
    for (i = 0; i < 0x10; i++) {
        idx[i] = (unsigned char)i;
        w = ((unsigned char *)g_goods_value)[(NATION(g_units[u]) << 4) + i] << 4;
        if (i == 0xf)
            w = 1;
        if (i == 0xe)
            w = 1;
        if (i == 8 && ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i] < 0x66)
            w = 0;
        val[i] = min_int_1028_e376(0x64, ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[i]) * w;
    }
    sort_int_pairs_by_key(0x10, idx, val);
    if (g_turn_nation == g_player_nation)
        set_current_nation(g_player_nation);
    for (i = 0xf; best < 0 && i >= 0; i--) {
        g = idx[i];
        if (g_turn_nation == g_player_nation && current_nation_boycotts(g) != 0)
            continue;
        if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[g] > 0)
            best = g;
    }
    if (best < 0) {
        viceroy_do_message(0x14, 3);
        goto out;
    }
    g = best;
    qty = min_int_1028_e376(0x64, ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[g]);
    ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[g] -= qty;
    unit_add_cargo(u, g, qty);
    ColChild_CancelMessage((&g_map_window), 1);
    viceroy_message(0x17);
    ColChild_AddNumberToMessage((&g_map_window), qty);
    ColChild_AddToMessage((&g_map_window), g_goods_name_word[g]);
    viceroy_message(0x19);
    ColChild_AddToMessage((&g_map_window), g_unit_types[g_units[u].type].name);
    viceroy_activate_message(1, 0x78L);
    r = 0;
out:
    return r;
}
