/* draw_econ_report_block -- shipped at 1058:4825.
 *
 * Ported by tools/port.py from matched/game/draw_econ_report_block.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_goods_sprites[][0x20];             SEG20:0x6526
 *   extern int far g_econ_title_word;                    SEG20:0xdd2c
 *   extern char far g_report_font;                       DGROUP:0x4b26
 *
 * The file's own Sized, ReportWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_2_1058_bcfb(Sized *o);
col_i16 get_w_2_1058_be8e(Sized *o);

#include "goods.h"
extern void set_current_nation(col_i16 n);
extern void TextColor(col_i16 v);
extern char * nth_text_string(col_i16 n);
extern void draw_string_shadowed(ReportWin *w, void *font, char *s, col_i16 x, col_i16 y, col_i16 size);
extern void GRPort_DrawLine(ReportWin *w, col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2);
extern void scale_map_select(col_i16 a, col_i16 b);
extern void sprite_draw(RECT *out, void *sprite, ReportWin *o, col_i16 x, col_i16 y);





void draw_econ_report_block(ReportWin *o, col_i16 nation)
{
    col_i16 y;
    RECT out;
    col_i16 i;
    col_i16 x;

    set_current_nation(nation);
    TextColor(0xf9);
    draw_string_shadowed(o, &g_report_font, nth_text_string(g_econ_title_word), get_w_2_1058_be8e((Sized *)o) / 2, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + 18, 16);
    x = 150;
    y = 40;
    TextColor(0xf9);
    GRPort_DrawLine(o, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x - 3, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + 40, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x - 3, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + 412);
    scale_map_select(7, 8);
    for (i = 0; i < GOODS_COUNT; i++) {
        sprite_draw(&out, (&g_goods_sprites[i]), o, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x + 13, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y);
        x += 30;
        GRPort_DrawLine(o, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x - 3, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + 40, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x - 3, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + 412);
    }
    scale_map_select(1, 1);
    for (i = 0; i < 18; i++)
        GRPort_DrawLine(o, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + 2, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + i * 20 + 72, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + 630, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + i * 20 + 72);
}
