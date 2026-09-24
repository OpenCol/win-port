/* europe_draw_market -- shipped at 1048:8fa0.
 *
 * Ported by tools/port.py from matched/game/europe_draw_market.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_goods_sprites[][0x20];             SEG20:0x6526
 *   extern char far g_boycott_sprite;                    SEG20:0x6946
 *   extern char far g_font_4b0e;                         DGROUP:0x4b0e
 *
 * The file's own EuropeWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 current_nation_boycotts(col_i16 bit);
col_i16 goods_price_buy(col_i16 g);
col_i16 goods_price_sell(col_i16 g);
void copy_rect_to_screen(EuropeWin *w, RECT *r);
void set_current_nation(col_i16 n);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

#include "goods.h"
extern void colony_blit_cell_1048_9d70(EuropeWin *w, col_i16 x, col_i16 y, col_i16 cx, col_i16 cy);
extern void scale_map_select(col_i16 a, col_i16 b);
extern void sprite_draw(RECT *out, void *rec, void *dst, col_i16 x, col_i16 y);
extern char * LTOA1(col_i16 v, char *s, col_i16 radix);

extern void TextColor(col_i16 v);
extern void draw_string_shadowed(EuropeWin *w, void *font, char *s,
                                     col_i16 x, col_i16 y, col_i16 a);


/* same object: 4-byte `push cs; call ` (rule 47) */






void europe_draw_market(EuropeWin *w, col_i16 refresh)
{
    col_i16 y;
    col_i16 cx;
    RECT r;
    RECT t1;
    RECT t2;
    RECT t3;
    char num[0x14];
    char buf[0x50];
    register col_i16 i;
    register col_i16 x;

    set_current_nation(w->nation);
    colony_blit_cell_1048_9d70(w, 0, 0x198, 0x262, 0x31);
    x = w->ox + 0x14;
    y = w->oy + 0x199;
    scale_map_select(7, 8);
    for (i = 0; i < GOODS_COUNT; i++) {
        cx = x;
        if (i == 0xa || i == 0xf) {
            scale_map_select(1, 1);
            sprite_draw(&t1, (&g_goods_sprites[i]), w, cx, y - 2);
            scale_map_select(7, 8);
        } else {
            sprite_draw(&t2, (&g_goods_sprites[i]), w, cx, y);
        }
        buf[0] = 0;
        LTOA1(goods_price_sell(i), num, 10);
        strcat(buf, num);
        strcat(buf, "/");
        LTOA1(goods_price_buy(i), num, 10);
        strcat(buf, num);
        TextColor(0);
        draw_string_shadowed(w, &g_font_4b0e, buf, cx, y + 0x24, 0);
        if (current_nation_boycotts(i) != 0)
            sprite_draw(&t3, &g_boycott_sprite, w, x, y);
        x += 0x26;
    }
    if (refresh != 0) {
        SetRect(&r, w->ox, w->oy + 0x196, w->ox + 0x262, w->oy + 0x1c9);
        copy_rect_to_screen(w, &r);
    }
    scale_map_select(1, 1);
}
