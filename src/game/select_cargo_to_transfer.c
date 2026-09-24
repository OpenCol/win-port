/* select_cargo_to_transfer -- shipped at 1058:8635.
 *
 * Ported by tools/port.py from matched/game/select_cargo_to_transfer.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern RouteRec far * far g_cur_route_rec;           DGROUP:0x4c18
 *   extern int far g_route_stop;                         DGROUP:0x4c22
 *   extern char far g_goods_sprites[][0x20];             SEG20:0x6526
 *
 * The file's own RouteRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_width_at_8_1058_c203(void *sprite);
void set_name_slot_1058_c2a5(col_i16 slot, void *s);

extern col_i16 route_stop_flag_nibble_get(col_i16 side);
extern col_i16 route_stop_nibble_get(col_i16 slot);
extern void route_stop_nibble_set(col_i16 slot, col_i16 good);
extern void route_stop_flag_nibble_set(col_i16 side, col_i16 n);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void * route_stop_name(col_i16 stop);

extern col_i16 choose_cargo(char *msg);

void select_cargo_to_transfer(col_i16 side, col_i16 x, col_i16 y)
{
    col_i16 px;
    col_i16 j;
    col_i16 base;
    col_i16 start;
    col_i16 i;
    col_i16 good;
    col_i16 n;
    col_i16 sel = -1;

    if (g_cur_route_rec->stops <= g_route_stop)
        goto done;
    base = side ? 6 : 0;
    start = side ? 0x1a9 : 0xe1;
    n = route_stop_flag_nibble_get(side);
    px = start;
    for (i = 0; i < n && sel < 0; i++) {
        good = route_stop_nibble_get(base + i);
        px += rect_width_at_8_1058_c203((&g_goods_sprites[good])) / 2 + 6;
        px += rect_width_at_8_1058_c203((&g_goods_sprites[good])) / 2 + 6;
        if (x < px)
            sel = i;
    }
    if (sel < 0 && n >= 6)
        goto done;
    if (sel >= 0) {
        for (j = sel; n - 1 > j; j++)
            route_stop_nibble_set(base + j, route_stop_nibble_get(base + j + 1));
        route_stop_flag_nibble_set(side, n - 1);
    } else {
        set_name_slot_1058_c2a5(0, route_stop_name(g_route_stop));
        if (side != 0)
            good = choose_cargo("CARGOLOAD");
        else
            good = choose_cargo("CARGOUNLOAD");
        if (good < 0)
            goto done;
        route_stop_nibble_set(base + n, good);
        route_stop_flag_nibble_set(side, n + 1);
    }
done:
    return;
}
