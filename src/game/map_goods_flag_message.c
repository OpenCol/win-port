/* map_goods_flag_message -- shipped at 1010:c0e5.
 *
 * Ported by tools/port.py from matched/game/map_goods_flag_message.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_colony_window;                     SEG20:0x25be
 *   extern char far g_map_window[];                      SEG20:0x0658
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void colony_activate_message(col_i16 kind, col_i32 ms);
void colony_message(col_i16 n);

extern void ColChild_CancelMessage(void *w, col_i16 a);
extern void ColChild_AddToMessage(void *w, col_i16 idx);




void map_goods_flag_message(col_i16 goods, col_i16 unused, col_i16 flag)
{
    ColChild_CancelMessage(&(*g_colony_window), 1);
    if (flag != 0)
        colony_message(0xf);
    else
        colony_message(0x10);
    ColChild_AddToMessage((&g_map_window), g_goods_name_word[goods]);
    colony_activate_message(1, 0L);
}
