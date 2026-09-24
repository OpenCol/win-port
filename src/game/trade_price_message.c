/* trade_price_message -- shipped at 1048:1add.
 *
 * Ported by tools/port.py from matched/game/trade_price_message.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern int far g_ctitle_word[];                      SEG20:0xd982
 *   extern int far g_cur_nation;                         DGROUP:0x4cc8
 *
 * The file's own Message dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void ColChild_AddNumberToMessage(Message *m, col_i16 v);
void ColChild_AddToMessage(Message *m, col_i16 idx);
void ColChild_BackspaceMessage(Message *m);
void ColChild_CancelMessage(Message *m, col_i16 a);
void add_nation_money_to_message(Message *m, col_i16 n);
void message_append(Message *m, char *s);
void msg_add_nation_word(Message *m, col_i16 i);

extern void set_current_nation(col_i16 n);
extern void msg_activate_redraw(Message *m, char kind, col_i32 arg);

/* same object: 4-byte `push cs; call ` */








void trade_price_message(Message *m, col_i16 goods, col_i16 price, col_i16 buy, col_i16 qty)
{
    set_current_nation(m->nation);
    ColChild_CancelMessage(m, 1);
    if (buy != 0)
        msg_add_nation_word(m, 13);
    else
        msg_add_nation_word(m, 14);
    ColChild_AddToMessage(m, g_goods_name_word[goods]);
    msg_add_nation_word(m, 12);
    ColChild_AddNumberToMessage(m, qty * price);
    ColChild_AddToMessage(m, g_ctitle_word[1]);
    message_append(m, "(");
    add_nation_money_to_message(m, g_cur_nation);
    ColChild_BackspaceMessage(m);
    message_append(m, ")");
    msg_activate_redraw(m, 1, 0L);
}
