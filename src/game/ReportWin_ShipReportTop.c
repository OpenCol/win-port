/* ReportWin_ShipReportTop -- shipped at 1058:6481.
 *
 * Ported by tools/port.py from matched/game/ReportWin_ShipReportTop.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_ship_report_title;                  SEG20:0xdd30
 *   extern int far g_ship_col1;                          SEG20:0xdd42
 *   extern int far g_ship_col2;                          SEG20:0xdd44
 *   extern int far g_ship_col3;                          SEG20:0xdd46
 *   extern int far g_ship_col4;                          SEG20:0xdd48
 *   extern char far g_report_font;                       DGROUP:0x4b26
 *   extern char far g_heading_font;                      DGROUP:0x4b22
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

extern void set_current_nation(col_i16 n);
extern void TextColor(col_i16 v);
extern char * nth_text_string(col_i16 n);
extern void say_string(char *s, col_i16 word);
extern void draw_string_shadowed(ReportWin *w, void *font, char *s, col_i16 x, col_i16 y, col_i16 size);
extern void GRPort_DrawLine(ReportWin *w, col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2);





void ReportWin_ShipReportTop(ReportWin *o, col_i16 nation)
{
    col_i16 y;
    col_i16 y2;
    char buf[0x50];
    col_i16 x;
    col_i16 i;

    TextColor(0xf9);
    draw_string_shadowed(o, &g_report_font, nth_text_string(g_ship_report_title), get_w_2_1058_be8e((Sized *)o) / 2, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + 18, 16);
    set_current_nation(nation);
    x = 2;
    y = 40;
    y2 = y + 2;
    buf[0] = 0;
    say_string(buf, g_ship_col1);
    TextColor(12);
    draw_string_shadowed(o, &g_heading_font, buf, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x + 75, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y2, 0);
    buf[0] = 0;
    say_string(buf, g_ship_col2);
    TextColor(12);
    draw_string_shadowed(o, &g_heading_font, buf, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x + 225, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y2, 0);
    buf[0] = 0;
    say_string(buf, g_ship_col3);
    TextColor(12);
    draw_string_shadowed(o, &g_heading_font, buf, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x + 375, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y2, 0);
    buf[0] = 0;
    say_string(buf, g_ship_col4);
    TextColor(12);
    draw_string_shadowed(o, &g_heading_font, buf, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x + 525, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + y2, 0);
    for (i = 1; i <= 3; i++) {
        TextColor(0xf9);
        GRPort_DrawLine(o, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x + i * 150, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + 40, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + x + i * 150, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + 368);
    }
    for (i = 0; i < 10; i++) {
        TextColor(0xf9);
        GRPort_DrawLine(o, (*((RECT *)((unsigned char *)(o) + 0x76a))).left + 2, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + i * 35 + 53, (*((RECT *)((unsigned char *)(o) + 0x76a))).right - 2, (*((RECT *)((unsigned char *)(o) + 0x76a))).top + i * 35 + 53);
    }
}
