/* ReportWin_TradeRoute1 -- shipped at 1058:7baa.
 *
 * Ported by tools/port.py from matched/game/ReportWin_TradeRoute1.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern RouteRec far * far g_cur_route_rec;           DGROUP:0x4c18
 *   extern char far g_goods_sprites[][0x20];             SEG20:0x6526
 *   extern char far g_font_small;                        DGROUP:0x4b1e
 *
 * The file's own ReportWin, RouteRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_width_1058_bd2d(RECT *r);
col_i16 rect_width_at_8_1058_c203(void *sprite);

extern void set_route(col_i16 stop);
extern char * route_stop_name(col_i16 stop);
extern col_i16 route_stop_flag_nibble_get(col_i16 dir);
extern col_i16 route_stop_nibble_get(col_i16 slot);
extern void say_number(char *s, col_i16 v);
extern void append_period_spaces(char *s);

extern void TextColor(col_i16 v);
extern void draw_string_shadowed(ReportWin *w, void *font, char *s, col_i16 x, col_i16 y, col_i16 size);
extern void sprite_draw(RECT *out, void *sprite, ReportWin *o, col_i16 x, col_i16 y);





void ReportWin_TradeRoute1(ReportWin *o, col_i16 y0)
{
    col_i16 y;
    col_i16 j;
    col_i16 i;
    col_i16 n;
    RECT rc1;
    RECT rc2;
    char buf[0x50];
    col_i16 x;
    col_i16 g;

    for (i = 0; g_cur_route_rec->stops > i; i++) {
        set_route(i);
        x = 10;
        y = y0;
        buf[0] = 0;
        say_number(buf, i + 1);
        append_period_spaces(buf);
        strcat(buf, route_stop_name(i));
        TextColor(0xf9);
        draw_string_shadowed(o, &(*g_font_small), buf, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y + 37, 1);
        x = 231;
        n = route_stop_flag_nibble_get(0);
        for (j = 0; j < n; j++) {
            g = route_stop_nibble_get(j);
            if (g >= 0) {
                x += rect_width_at_8_1058_c203((&g_goods_sprites[g])) / 2 + 6;
                sprite_draw(&rc1, (&g_goods_sprites[g]), o, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y + 27);
                x += rect_width_at_8_1058_c203((&g_goods_sprites[g])) / 2 + 6;
            }
        }
        x = 431;
        n = route_stop_flag_nibble_get(1);
        for (j = 0; j < n; j++) {
            g = route_stop_nibble_get(j + 6);
            if (g >= 0) {
                x += rect_width_at_8_1058_c203((&g_goods_sprites[g])) / 2 + 6;
                sprite_draw(&rc2, (&g_goods_sprites[g]), o, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y + 27);
                x += rect_width_at_8_1058_c203((&g_goods_sprites[g])) / 2 + 6;
            }
        }
        y0 += 75;
    }
}
