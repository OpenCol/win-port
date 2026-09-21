/* colony_mark_export -- shipped at 1018:045e.
 *
 * Ported by tools/port.py from matched/game/colony_mark_export.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_prod[];                             SEG21:0x0122
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0xca bytes */


extern col_i16 colony_warehouse_capacity(void);

void colony_mark_export(col_i16 goods, col_i16 flag)
{
    if (colony_warehouse_capacity() <= ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[goods])
        flag = 0;
    if (g_prod[goods] != 0)
        flag = 0;
    if (flag != 0)
        g_cur_colony_rec->export_goods = goods;
    else if (g_cur_colony_rec->export_goods == (char)goods)
        g_cur_colony_rec->export_goods = -1;
}
