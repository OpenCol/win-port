/* orders_menu_init -- shipped at 1048:a959.
 *
 * Ported by tools/port.py from matched/game/orders_menu_init.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_order_flags[];            SEG30:0x0050
 *   extern char g_text_405[];                            DGROUP:0x2357
 *   extern char g_text_406[];                            DGROUP:0x2366
 *   extern char g_text_411[];                            DGROUP:0x2374
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void menu_enable_item_1048_e2dd(void *menu, col_i32 id, col_i16 on);
extern void menu_call_da9f(void *menu, col_i32 id, char *text);





void orders_menu_init(Win *w)
{
    col_i32 i = 400;

    for (; i < 417; i++)
        menu_enable_item_1048_e2dd(((char *)((unsigned char *)(w) + 0x8c)), i, 1);
    for (i = 0; i < 17; i++)
        g_order_flags[i] = 1;
    menu_call_da9f(((char *)((unsigned char *)(w) + 0x8c)), 405L, g_text_405);
    menu_call_da9f(((char *)((unsigned char *)(w) + 0x8c)), 406L, g_text_406);
    menu_call_da9f(((char *)((unsigned char *)(w) + 0x8c)), 411L, g_text_411);
}
