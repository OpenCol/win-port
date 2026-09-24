/* colony_load_unit -- shipped at 1010:d7fa.
 *
 * Ported by tools/port.py from matched/game/colony_load_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern char far g_colony_window;                     SEG20:0x25be
 *   extern char far g_text_ctx;                          DGROUP:0x009e
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_cargo_amount;                       DGROUP:0x4bd2
 *   extern int far g_b2;                                 DGROUP:0x00b2
 *   extern int far g_carrier_count;                      DGROUP:0x0c14
 *   extern int far g_text_args;                          SEG31:0x02d4
 *
 * The file's own Unit, UnitType, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1010_e5d0(col_i16 a, col_i16 b);
void colony_activate_message(char kind, col_i32 arg);
void colony_message(col_i16 idx);
void colony_window_refresh(void);
void set_text_arg_1010_ebb0(col_i16 slot, col_i16 v);

/* 0xca */










extern col_i16 unit_cargo_room(col_i16 u, col_i16 goods, col_i16 *room);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern col_i16 prompt_text_number(char *ctx, char *name, col_i16 n);
extern col_i16 minimax(col_i16 v, col_i16 lo, col_i16 hi);
extern col_i16 colony_load_goods(col_i16 u, col_i16 goods, col_i16 max);
extern void ColChild_CancelMessage(void *w, col_i16 a);
extern void ColChild_AddNumberToMessage(void *w, col_i16 v);
extern void ColChild_AddToMessage(void *w, col_i16 word);
extern void ColChild_BackspaceMessage(void *w);
extern void message_append(void *w, char *s);

/* same object: reached by the 4-byte `push cs; call ` */






col_i16 colony_load_unit(register col_i16 u, register col_i16 goods, col_i16 enabled, col_i16 ask)
{
    col_i16 r;
    col_i16 got;
    col_i16 n;
    col_i16 cap;

    r = 1;
    if (((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[goods] < 1) {
        ColChild_CancelMessage(&(*g_colony_window), 1);
        colony_message(6);
        ColChild_AddToMessage(&(*g_colony_window), g_goods_name_word[goods]);
        colony_message(7);
        colony_activate_message(3, 0x78L);
        goto out;
    }
    if (g_b2 != 0 && g_carrier_count == 0) {
        ColChild_CancelMessage(&(*g_colony_window), 1);
        colony_message(5);
        ColChild_AddToMessage(&(*g_colony_window), g_goods_name_word[goods]);
        ColChild_BackspaceMessage(&(*g_colony_window));
        message_append(&(*g_colony_window), "!");
        colony_activate_message(3, 0x78L);
        goto out;
    }
    if (enabled == 0) {
        r = 0;
        goto out;
    }
    if (unit_cargo_room(u, goods, &n) == 0) {
        ColChild_CancelMessage(&(*g_colony_window), 1);
        colony_message(4);
        ColChild_AddToMessage(&(*g_colony_window), g_goods_name_word[goods]);
        ColChild_BackspaceMessage(&(*g_colony_window));
        message_append(&(*g_colony_window), "!");
        colony_activate_message(3, 0x78L);
        goto out;
    }
    n = min_int_1010_e5d0(n, 100);
    n = min_int_1010_e5d0(n, ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[goods]);
    if (ask != 0) {
        set_name_slot_from_text(0, g_goods_name_word[goods]);
        set_name_slot_from_text(1, g_unit_types[g_units[u].type].name);
        set_text_arg_1010_ebb0(0, n);
        if (prompt_text_number(&(*g_text_ctx), "HOWMUCH1", n) != 0)
            goto out;
        cap = minimax((*g_text_args), 0, n);
        n = min_int_1010_e5d0(cap, n);
        if (n <= 0)
            goto out;
    }
    got = colony_load_goods(u, goods, n);
    ColChild_CancelMessage(&(*g_colony_window), 1);
    ColChild_AddNumberToMessage(&(*g_colony_window), g_cargo_amount);
    ColChild_AddToMessage(&(*g_colony_window), g_goods_name_word[goods]);
    colony_message(2);
    ColChild_AddToMessage(&(*g_colony_window), g_unit_types[g_units[u].type].name);
    colony_activate_message(1, 0x78L);
    if (g_b2 != 0)
        colony_window_refresh();
    r = 0;
out:
    return r;
}
