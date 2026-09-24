/* draw_econ_report_paged -- shipped at 1058:49ea.
 *
 * Ported by tools/port.py from matched/game/draw_econ_report_paged.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern int far g_thousands_word;                     SEG20:0xdd3a
 *   extern char far g_goods_sprites[][0x20];             SEG20:0x6526
 *   extern int far g_econ_title_word;                    SEG20:0xdd2c
 *   extern char far g_report_font;                       DGROUP:0x4b26
 *   extern char far g_heading_font;                      DGROUP:0x4b22
 *   extern char far g_number_font;                       DGROUP:0x4b1a
 *
 * The file's own Sized, SButton, ReportWin, Colony dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: shown -> f1a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 port_width_1058_bcfb(Sized *o);
col_i16 port_width_1058_be8e(Sized *o);
void ctl_flag_clear(SButton *b);
void ctl_flag_set(SButton *b);
void draw_econ_report_block(ReportWin *o, char nation);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

#include "goods.h"
extern void set_current_colony(col_i16 c);
extern void set_current_nation(col_i16 n);
extern void TextColor(col_i16 v);
extern char * nth_text_string(col_i16 n);
extern void draw_string_shadowed(ReportWin *w, void *font, char *s, col_i16 x, col_i16 y, col_i16 size);
extern void GRPort_DrawLine(ReportWin *w, col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2);
extern void GRPort_DrawRaisedRect(ReportWin *w, RECT *r);
extern void scale_map_select(col_i16 a, col_i16 b);
extern void sprite_draw(RECT *out, void *sprite, ReportWin *o, col_i16 x, col_i16 y);

extern void invalidate_hwnd(col_i16 h);
extern char * LTOA1(col_i16 v, char *s, col_i16 radix);
extern void append_space(char *s);
extern void say_string(char *buf, col_i16 word);











void draw_econ_report_paged(ReportWin *o, char nation)
{
    col_i16 x;
    col_i16 y;
    col_i16 xx;
    col_i16 c;
    col_i16 rows;
    col_i16 color;
    col_i16 g;
    col_i16 count;
    RECT rc;
    char buf[0x50];
    col_i16 xs;
    col_i16 q;

    count = 0;
    for (c = 0; g_colony_count > c; c++) {
        set_current_colony(c);
        if (g_cur_colony_rec->nation == nation)
            count++;
    }
    if (count / 17 > (*((col_i16 *)((unsigned char *)(o) + 0x7f2))))
        ctl_flag_set((*((SButton * *)((unsigned char *)(o) + 0x8b0))));
    else
        ctl_flag_clear((*((SButton * *)((unsigned char *)(o) + 0x8b0))));
    ctl_flag_set((*((SButton * *)((unsigned char *)(o) + 0x8ac))));
    SetRect(&rc, (*((RECT *)((unsigned char *)(o) + 0x76a))).left, (*((RECT *)((unsigned char *)(o) + 0x76a))).bottom - 40, (*((RECT *)((unsigned char *)(o) + 0x76a))).right, (*((RECT *)((unsigned char *)(o) + 0x76a))).bottom);
    GRPort_DrawRaisedRect(o, &rc);
    draw_econ_report_block(o, nation);
    x = 2;
    y = 79;
    rows = 0;
    count = -1;
    for (c = 0; g_colony_count > c; c++) {
        set_current_colony(c);
        if (g_cur_colony_rec->nation == nation)
            count++;
        else
            continue;
        if ((*((col_i16 *)((unsigned char *)(o) + 0x7f2))) * 17 - 17 > count || (*((col_i16 *)((unsigned char *)(o) + 0x7f2))) * 17 < count)
            continue;
        TextColor(0xfc);
        draw_string_shadowed(o, &g_heading_font, (char *)g_cur_colony_rec + 2, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y + 2, 1);
        xx = 145;
        for (g = 0; g < 16; g++) {
            q = ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[g];
            LTOA1(q, buf, 10);
            color = 0;
            if (q != 0)
                color = 12;
            if (q >= 100)
                color = 0xfc;
            xs = xx;
            if (q < 1000)
                xs += 4;
            if (q < 100)
                xs += 4;
            if (q < 10)
                xs += 4;
            if (q >= 1000) {
                LTOA1(q / 1000, buf, 10);
                append_space(buf);
                say_string(buf, g_thousands_word);
                color = 0xff;
                xs += 4;
            }
            TextColor(color);
            draw_string_shadowed(o, &g_number_font, buf, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + xs, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y + 2, 1);
            xx += 30;
        }
        rows++;
        y += 20;
    }
    if ((*((col_i16 *)((unsigned char *)(o) + 0x7f2))) * 17 - 17 >= count) {
        (*((col_i16 *)((unsigned char *)(o) + 0x7f2)))--;
        draw_econ_report_paged(o, nation);
        return;
    }
}
