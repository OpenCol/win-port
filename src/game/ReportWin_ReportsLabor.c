/* ReportWin_ReportsLabor -- shipped at 1058:2dcd.
 *
 * Ported by tools/port.py from matched/game/ReportWin_ReportsLabor.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern SpecRec far g_spec_recs[];                    SEG20:0xd61a
 *   extern char far g_icon_sprites[][0x20];              SEG20:0x6246
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int far g_labor_title_word;                   SEG20:0xdd2a
 *   extern char far g_report_font;                       DGROUP:0x4b26
 *   extern char far g_heading_font;                      DGROUP:0x4b22
 *   extern char far g_message_buf[];                     DGROUP:0x4e6c
 *   extern char far g_number_buf[];                      DGROUP:0x566c
 *
 * The file's own Sized, GameFont, ReportWin, Colony, Unit, SpecRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * message_buf_1058_bec6(void);
col_i16 font_height_1058_be4f(GameFont *f);
col_i16 font_text_extent_1058_be29(GameFont *f, char *s);
col_i16 port_width_1058_bcfb(Sized *o);
col_i16 port_width_1058_be8e(Sized *o);
col_i16 rect_height_1058_bd4c(RECT *r);
col_i16 rect_width_1058_bd2d(RECT *r);
void * set_handler_slot_0_1058_bf13(void *o, void *h);
void * set_handler_slot_12_1058_bdb7(void *o, void *h);
void message_buf_clear(void);
void say_number_1058_bede(col_i16 n);

#include "units.h"
/* 0x1c bytes at SEG20:0xa208 */
/* 8 bytes, see jobs.h */
typedef struct { void *slot; } Slot0;
typedef struct { char pad[12]; void *slot; } Slot12;











extern void scale_map_select(col_i16 a, col_i16 b);
extern void set_current_nation(char n); /* the module passes the nation as a char */
extern void set_current_colony(col_i16 c);
extern void TextColor(col_i16 v);
extern char * nth_text_string(col_i16 n);
extern void draw_string_shadowed(ReportWin *w, void *font, char *s, col_i16 x, col_i16 y, col_i16 size);
extern void * memset_rtl(void *p, col_i16 c, col_u16 n);
extern col_i16 colonist_specialty_get(col_i16 k);
extern col_i16 unit_is_colonist(col_i16 u);
extern col_i16 specialist_portrait(col_i16 spec);
extern void sprite_draw9(RECT *out, void *sprite, ReportWin *o, col_i16 x, col_i16 y, unsigned char c);
extern void sprite_draw(RECT *out, void *sprite, ReportWin *o, col_i16 x, col_i16 y);
extern void say_string(char *buf, col_i16 word);
extern col_i16 text_extent(col_i16 hfont, char *s);
extern char * LTOA1(col_i16 v, char *s, col_i16 radix);

extern void labor_report_hook(void);
extern void on_labor_report_click(void);























void ReportWin_ReportsLabor(ReportWin *o, char nation)
{
    col_i16 x0;
    col_i16 y0;
    col_i16 x;
    col_i16 y;
    col_i16 tx;
    col_i16 ty;
    col_i16 k;
    col_i16 sp;
    col_i16 n;
    col_i16 col;
    col_i16 w;
    col_i16 colw;
    col_i16 rowh;
    RECT rc1;
    RECT rc2;
    col_i16 i;
    col_i16 s;

    scale_map_select(1, 1);
    set_current_nation(nation);
    TextColor(0xf9);
    draw_string_shadowed(o, &g_report_font, nth_text_string(g_labor_title_word), port_width_1058_be8e((Sized *)o) / 2, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + 18, 16);
    x0 = 25;
    y0 = 40;
    memset_rtl(((col_i16 *)((unsigned char *)(o) + 0x872)), 0, 0x3a);
    for (i = 0; i < 29; i++) {
        ((col_i16 *)((unsigned char *)(o) + 0x7fe))[i] = rect_width_1058_bd2d(&(*((RECT *)((unsigned char *)(o) + 0x76a))));
        ((col_i16 *)((unsigned char *)(o) + 0x838))[i] = rect_height_1058_bd4c(&(*((RECT *)((unsigned char *)(o) + 0x76a))));
    }
    for (i = 0; g_colony_count > i; i++) {
        set_current_colony(i);
        if (g_cur_colony_rec->nation != nation)
            continue;
        for (k = 0; g_cur_colony_rec->pop > k; k++) {
            sp = colonist_specialty_get(k);
            ((col_i16 *)((unsigned char *)(o) + 0x872))[sp]++;
        }
    }
    for (s = 0; g_unit_count > s; s++) {
        if ((char)(g_units[s].flags & UNIT_NATION_MASK) != nation)
            continue;
        if (unit_is_colonist(s) != 0)
            ((col_i16 *)((unsigned char *)(o) + 0x872))[g_units[s].spec]++;
    }
    ((col_i16 *)((unsigned char *)(o) + 0x872))[0x13] += ((col_i16 *)((unsigned char *)(o) + 0x872))[0x1c];
    y = y0;
    x = x0;
    n = 0;
    col = 0;
    colw = 210;
    rowh = 37;
    for (i = 0; i < 29; i++) {
        s = i;
        if (s == 0x13)
            continue;
        if (s == 0x17)
            continue;
        if (s == 0x12)
            continue;
        if (s == 0x1c)
            s = 0x13;
        ((col_i16 *)((unsigned char *)(o) + 0x7fe))[s] = (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x - 15;
        ((col_i16 *)((unsigned char *)(o) + 0x838))[s] = (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y;
        TextColor(0);
        sprite_draw9(&rc1, (&g_icon_sprites[specialist_portrait(s)]), o, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x - 3, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y, 0);
        sprite_draw(&rc2, (&g_icon_sprites[specialist_portrait(s)]), o, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y);
        tx = x + 20;
        ty = y + 7;
        message_buf_clear();
        say_string(message_buf_1058_bec6(), g_spec_recs[s].expert);
        w = font_text_extent_1058_be29((GameFont *)&g_heading_font, message_buf_1058_bec6());
        TextColor(0);
        draw_string_shadowed(o, &g_heading_font, message_buf_1058_bec6(), (*((RECT *)((unsigned char *)(o) + 0x76a))).left + tx, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + ty, 1);
        tx = tx + w;
        ty += font_height_1058_be4f((GameFont *)&g_heading_font) + 1;
        message_buf_clear();
        say_number_1058_bede(((col_i16 *)((unsigned char *)(o) + 0x872))[s]);
        tx = x + 47;
        TextColor(0);
        draw_string_shadowed(o, &g_heading_font, message_buf_1058_bec6(), (*((RECT *)((unsigned char *)(o) + 0x76a))).left + tx, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + ty, 0);
        y += rowh;
        n++;
        if ((col != 0) + 8 <= n && col < 2) {
            n = 0;
            col++;
            y = y0;
            x += colw;
        }
    }
    set_handler_slot_0_1058_bf13((Slot0 *)((char *)o + 0x33), (void *)labor_report_hook);
    set_handler_slot_12_1058_bdb7((Slot12 *)((char *)o + 0x33), (void *)on_labor_report_click);
}
