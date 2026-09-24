/* colony_buy_build -- shipped at 1010:7388.
 *
 * Ported by tools/port.py from matched/game/colony_buy_build.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern char g_buy_busy;                              DGROUP:0x0a05
 *   extern char far g_msg_name[];                        SEG31:0x00a4
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * The file's own ColChild, Colony, Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1010_e632(col_i16 a, col_i16 b);
void colony_window_refresh(void);
void control_show_1010_e574(void *c);

#include "goods.h"
extern void ResetTimer(col_i16 id);
extern void set_current_colony(col_i16 c);
extern col_i16 build_item_kind(col_i16 item, col_i16 *idx);
extern col_i16 build_item_cost(col_i16 item, col_i16 *tools);
extern char * build_item_name(col_i16 item);

extern col_i32 nation_gold(col_i16 nation);
extern col_i16 text_message_show(char *msg, col_i16 n);
extern void nation_take_gold(col_i16 nation, col_i32 amount);
/* same object: 4-byte `push cs; call ` (rule 47) */




void colony_buy_build(ColChild *w, col_i16 unused1)
{
    col_i16 idx;
    col_i16 kind;
    col_i16 cost;
    col_i16 hshort;
    col_i16 r;
    col_i16 extra;
    col_i16 tools;
    register col_i16 price;
    register col_i16 tshort;

    if (w->timer != -1) {
        ResetTimer(w->timer);
        w->timer = -1;
    }
    control_show_1010_e574((char *)w + 0x972);
    if (g_buy_busy != 0)
        return;
    g_buy_busy = 1;
    set_current_colony(w->colony);
    kind = build_item_kind(g_colonies[w->colony].item, &idx);
    cost = build_item_cost(g_colonies[w->colony].item, &tools);
    tshort = tools - ((col_i16 *)((unsigned char *)&(g_colonies[w->colony]) + 0x9a))[GOODS_TOOLS];
    tshort = max_int_1010_e632(0, tshort);
    if (kind == 0)
        goto done;
    hshort = cost - g_colonies[w->colony].hammers;
    hshort = max_int_1010_e632(0, hshort);
    if (hshort <= 0 && tshort == 0)
        goto done;
    price = hshort * 13;
    if (tshort != 0)
        price += (g_nations[g_colonies[w->colony].nation].f5a + 4) * tshort;
    if (g_colonies[w->colony].hammers == 0)
        price = price * 2;
    strcpy(g_msg_name, build_item_name(g_colonies[w->colony].item));
    g_text_args[0] = (col_i32)price;
    g_text_args[1] = nation_gold(g_colonies[w->colony].nation);
    if (nation_gold(g_colonies[w->colony].nation) >= (col_i32)price)
        r = text_message_show("BUYME1", 5);
    else {
        text_message_show("BUYME0", 5);
        r = 0;
    }
    if (r != 2)
        goto done;
    extra = cost - g_colonies[w->colony].hammers;
    if (extra > 0)
        g_colonies[w->colony].f98 += extra;
    g_colonies[w->colony].hammers = cost;
    nation_take_gold(g_colonies[w->colony].nation, (col_i32)price);
    if (tools != 0)
        ((col_i16 *)((unsigned char *)&(g_colonies[w->colony]) + 0x9a))[GOODS_TOOLS] += tshort;
    colony_window_refresh();
done:
    g_buy_busy = 0;
}
