/* InfoCargo -- shipped at 1058:8dd6.
 *
 * Ported by tools/port.py from matched/game/InfoCargo.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern unsigned char far g_goods_value[];            SEG30:0x0010
 *   extern char far g_icon_sprites[][0x20];              SEG20:0x6246
 *   extern float g_scale_denom;                          DGROUP:0x2876
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_2_1058_bcfb(void *w);
col_i16 rect_width_at_8_1058_c203(void *s);

#include "goods.h"
#include "units.h"
extern col_i16 unit_cargo_type(col_i16 u, col_i16 slot);
extern col_i16 unit_cargo_amount(col_i16 u, col_i16 slot);
extern void sort_int_pairs_by_key(col_i16 n, col_i16 *idx, col_i16 *key);
extern void scale_map_select(col_i16 a, col_i16 b);
extern void sprite_draw(void *out, void *rec, void *dst, col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` (rule 47) */



void InfoCargo(void *port, col_i16 u, col_i16 x, col_i16 y)
{
    col_i16 i;
    col_i16 amount;
    col_i16 icon;
    col_i16 n;
    col_i16 v;
    col_i16 zoom;
    col_i16 slot;
    col_i16 prev;
    register col_i16 type;
    register col_i16 w;

    n = g_units[u].cargo_count;
    {
    RECT r1;
    RECT r2;
    col_i16 mx;
    col_i16 idx[6];
    col_i16 key[6];
    for (i = 0; i < n; i++) {
        idx[i] = (unsigned char)i;
        type = unit_cargo_type(u, i);
        amount = unit_cargo_amount(u, i);
        v = ((unsigned char *)g_goods_value)[((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) << 4) + type] << 4;
        if (type == GOODS_MUSKETS)
            v = 0;
        if (type == GOODS_TOOLS)
            v = 0;
        if (type == GOODS_HORSES)
            v = 1;
        key[i] = v * amount;
    }
    sort_int_pairs_by_key(n, idx, key);
    zoom = 8;
    prev = -1;
again:
    mx = x + 0xe;
    for (i = 0; i < n; i++) {
        slot = idx[i];
        type = unit_cargo_type(u, slot);
        amount = unit_cargo_amount(u, slot);
        if (amount <= 0)
            continue;
        icon = amount >= 100 ? 0x17 : 0x27;
        if (type != GOODS_CIGARS && type != GOODS_MUSKETS)
            w = (col_i16)(rect_width_at_8_1058_c203((&g_icon_sprites[icon + type])) * (zoom / g_scale_denom));
        else
            w = (col_i16)(rect_width_at_8_1058_c203((&g_icon_sprites[icon + type])) * 1.0);
        w = w / 2;
        mx += w;
        if (type != GOODS_CIGARS && type != GOODS_MUSKETS)
            w = (col_i16)(rect_width_at_8_1058_c203((&g_icon_sprites[icon + type])) * (zoom / g_scale_denom));
        else
            w = (col_i16)(rect_width_at_8_1058_c203((&g_icon_sprites[icon + type])) * 1.0);
        w = w / 2;
        mx += w + 6;
    }
    if (x + 0xe == mx)
        goto done;
    if (get_w_2_1058_bcfb(port) - 0x14 < mx && mx > 1)
        zoom--;
    else
        goto done;
    if (prev == mx)
        goto done;
    prev = mx;
    goto again;
done:
    scale_map_select(zoom, 8);
    x += 0xe;
    for (i = 0; i < n; i++) {
        slot = idx[i];
        type = unit_cargo_type(u, slot);
        amount = unit_cargo_amount(u, slot);
        if (amount <= 0)
            continue;
        icon = amount >= 100 ? 0x17 : 0x27;
        if (type != GOODS_CIGARS && type != GOODS_MUSKETS)
            w = (col_i16)(rect_width_at_8_1058_c203((&g_icon_sprites[icon + type])) * (zoom / g_scale_denom));
        else
            w = (col_i16)(rect_width_at_8_1058_c203((&g_icon_sprites[icon + type])) * 1.0);
        w = w / 2;
        x += w;
        if (type != GOODS_CIGARS && type != GOODS_MUSKETS) {
            sprite_draw(&r1, (&g_icon_sprites[icon + type]), port, x, y);
            w = (col_i16)(rect_width_at_8_1058_c203((&g_icon_sprites[icon + type])) * (zoom / g_scale_denom));
        } else {
            scale_map_select(1, 1);
            sprite_draw(&r2, (&g_icon_sprites[icon + type]), port, x, y);
            w = (col_i16)(rect_width_at_8_1058_c203((&g_icon_sprites[icon + type])) * 1.0);
            scale_map_select(zoom, 8);
        }
        w = w / 2;
        x += w + 6;
    }
    scale_map_select(1, 1);
    }
}
