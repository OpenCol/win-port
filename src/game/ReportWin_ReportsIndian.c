/* ReportWin_ReportsIndian -- shipped at 1058:0cab.
 *
 * Ported by tools/port.py from matched/game/ReportWin_ReportsIndian.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Settlement far g_settlements[];               SEG20:0xcb9a
 *   extern LevelRec far g_level[];                       SEG20:0xcb7c
 *   extern Sprite far g_mood_art[];                      SEG20:0x7086
 *   extern int far g_settlement_count;                   SEG20:0x779c
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern unsigned char far g_tribe_colour[];           SEG20:0xd9ce
 *   extern int far g_goods_name_word[];                  SEG20:0xd86e
 *   extern int far g_misc_words[];                       SEG20:0xdcc8
 *   extern unsigned char far g_nation_colour[];          DGROUP:0x009a
 *   extern int far g_draw_color;                         DGROUP:0x337e
 *   extern char far g_report_font_small;                 DGROUP:0x4b0a
 *   extern char far g_font_small;                        DGROUP:0x4b1e
 *   extern char far g_report_font;                       DGROUP:0x4b26
 *   extern Tribe far * far g_cur_tribe_rec;              DGROUP:0x4c28
 *   extern Settlement far * far g_cur_settlement_rec;    DGROUP:0x4c2c
 *   extern int far g_cur_tribe_nation;                   DGROUP:0x4c32
 *
 * The file's own Sprite, ReportWin, Unit, Tribe, LevelRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 font_text_extent_1058_be29(void *f, const char *s);

#include "goods.h"
#include "settle.h"
#include "units.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))
extern void TextColor(unsigned char v);
extern char * nth_text_string(col_i16 n);
extern void draw_string_shadowed(ReportWin *w, void *font, char *s, col_i16 x,
                                     col_i16 y, col_i16 size);
extern void set_current_tribe(col_i16 t);
extern col_i16 nation_pair_flags_get(col_i16 a, col_i16 b);
extern col_i16 table_78_word(col_i16 a, col_i16 tribe);
extern col_i16 quarter_of_100(col_i16 v);
extern col_i16 minimax(col_i16 a, col_i16 lo, col_i16 hi);
extern void sprite_draw(RECT *out, Sprite *s, ReportWin *dst, col_i16 x, col_i16 y);
extern col_i16 get_adjective(col_i16 n);
extern void say_string(char *buf, col_i16 word);
extern void say_number(char *buf, col_i16 n);
extern void append_space(char *s);
extern void append_colon_space(char *s);
extern void set_current_native_settlement(col_i16 s);

/* same object: 4-byte `push cs; call ` (rule 47) */


void ReportWin_ReportsIndian(ReportWin *w, col_i16 nation)
{
    col_i16 i;
    col_i16 x0;
    col_i16 y0;
    col_i16 y;
    col_i16 tnation;
    col_i16 mood;
    col_i16 face;
    col_i16 villages;
    col_i16 missions;
    col_i16 braves;
    char colour;
    RECT r;
    char buf[0x50];
    register col_i16 s;
    register col_i16 x;

    TextColor(0xa);
    draw_string_shadowed(w, &g_report_font, nth_text_string(g_misc_words[29]), w->ox + 0x140,
                         w->oy + 0xf, 0x10);
    x0 = 0xa;
    y0 = 0x2d;
    for (i = 0; i < 8; i++) {
        set_current_tribe(i);
        tnation = g_cur_tribe_nation;
        if ((nation_pair_flags_get(nation, tnation) & 0x20)
            || (g_cur_tribe_rec->flags & 0x80)) {
            x = x0;
            y = y0;
            mood = quarter_of_100(table_78_word(i, nation));
            face = (table_78_word(i, nation) - mood * 0x19) / 5;
            if (mood > 0)
                face = 4 - face;
            face = minimax(face, 0, 4);
            if ((nation_pair_flags_get(nation, tnation) & 0x40) == 0)
                ;
            if (g_cur_tribe_rec->flags & 0x80)
                mood = 3;
            sprite_draw(&r, &g_mood_art[mood], w, w->ox + x, w->oy + y);
            x += 0x28;
            y += 6;
            colour = tnation < 4 ? g_nation_colour[tnation] : g_tribe_colour[tnation - 4];
            buf[0] = 0;
            say_string(buf, get_adjective(tnation));
            append_colon_space(buf);
            if (g_cur_tribe_rec->flags & 0x80) {
                say_string(buf, g_misc_words[130]);
                TextColor(colour);
                draw_string_shadowed(w, &(*g_font_small), buf, w->ox + x, w->oy + y, 0x15);
                x += font_text_extent_1058_be29(&(*g_font_small), buf) + 2;
            } else {
                TextColor(colour);
                draw_string_shadowed(w, &(*g_font_small), buf, w->ox + x, w->oy + y, 0x15);
                x += font_text_extent_1058_be29(&(*g_font_small), buf) + 2;
            }
            if (!(g_cur_tribe_rec->flags & 0x80)) {
                buf[0] = 0;
                say_string(buf, g_level[g_cur_tribe_rec->level].name);
                x = 0x276 - font_text_extent_1058_be29(&(*g_font_small), buf);
                TextColor(colour);
                draw_string_shadowed(w, &(*g_font_small), buf, w->ox + x, w->oy + y, 0x15);
                x = x0 + 0x3c;
                y += 0x14;
                villages = 0;
                missions = 0;
                for (s = 0; s < g_settlement_count; s++) {
                    if (g_settlements[s].owner == (unsigned char)tnation) {
                        set_current_native_settlement(s);
                        villages++;
                        if (((col_i16)g_cur_settlement_rec->mission & MISSION_NATION_MASK) == (char)nation)
                            missions++;
                    }
                }
                braves = g_cur_tribe_rec->muskets;
                for (s = 0; s < g_unit_count; s++) {
                    if (NATION(g_units[s]) == (unsigned char)tnation
                        && (g_units[s].type == UNIT_ARMED_BRAVES || g_units[s].type == UNIT_MOUNTED_WARRIORS))
                        braves++;
                }
                braves *= 0x32;
                buf[0] = 0;
                say_number(buf, villages);
                append_space(buf);
                if (villages == 1)
                    say_string(buf, g_level[g_cur_tribe_rec->level].settlement);
                else
                    say_string(buf, g_level[g_cur_tribe_rec->level].settlements);
                TextColor(g_draw_color);
                draw_string_shadowed(w, &g_report_font_small, buf, w->ox + x, w->oy + y, 5);
                x += 0x70;
                if (missions != 0) {
                    buf[0] = 0;
                    say_number(buf, missions);
                    append_space(buf);
                    if (missions == 1)
                        say_string(buf, g_misc_words[27]);
                    else
                        say_string(buf, g_misc_words[28]);
                    TextColor(g_draw_color);
                    draw_string_shadowed(w, &g_report_font_small, buf, w->ox + x, w->oy + y, 5);
                }
                x += 0x70;
                if (braves != 0) {
                    buf[0] = 0;
                    say_number(buf, braves);
                    append_space(buf);
                    say_string(buf, g_goods_name_word[GOODS_MUSKETS]);
                    TextColor(g_draw_color);
                    draw_string_shadowed(w, &g_report_font_small, buf, w->ox + x, w->oy + y, 5);
                }
                x += 0x70;
                if (g_cur_tribe_rec->horse_taken != 0) {
                    buf[0] = 0;
                    say_number(buf, (col_i16)g_cur_tribe_rec->horse_taken);
                    append_space(buf);
                    say_string(buf, g_misc_words[45]);
                    TextColor(g_draw_color);
                    draw_string_shadowed(w, &g_report_font_small, buf, w->ox + x, w->oy + y, 5);
                }
            }
            y0 += 0x28;
        }
    }
}
