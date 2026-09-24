/* europe_draw_status -- shipped at 1048:8ded.
 *
 * Ported by tools/port.py from matched/game/europe_draw_status.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *   extern int far g_nation_name_word[];                 SEG20:0xd8ae
 *   extern int far g_nation_name[];                      SEG20:0xd896
 *   extern int far g_word_d994;                          SEG20:0xd994
 *   extern int far g_word_d984;                          SEG20:0xd984
 *   extern int far g_season;                             SEG20:0x778e
 *   extern int far g_year;                               SEG20:0x778c
 *   extern int far g_season_word[];                      DGROUP:0x4b34
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *
 * The file's own Nation, EuropeWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_current_nation(col_i16 n);
void sub_1048_d48b(EuropeWin *w, char *s);

/* 0x13c bytes */
extern char * nth_text_string(col_i16 n);


extern void append_comma_space(char *s);
extern void append_period_spaces(char *s);
extern void append_percent(char *s);
extern void append_space(char *s);
extern void say_string(char *s, col_i16 word);
extern void say_number(char *s, col_i16 v);
extern void say_nation_gold(col_i16 nation, char *buf);
extern char * LTOA1(col_i16 v, char *s, col_i16 radix);

/* same object: 4-byte `push cs; call ` (rule 47) */



void europe_draw_status(EuropeWin *w)
{
    char buf[0x50];
    char num[0x14];

    if (w->f776 != -1)
        return;
    set_current_nation(w->nation);
    buf[0] = 0;
    num[0] = 0;
    strcpy(buf, nth_text_string(g_nation_name_word[g_cur_nation]));
    append_comma_space(buf);
    strcat(buf, nth_text_string(g_nation_name[g_cur_nation]));
    append_period_spaces(buf);
    strcat(buf, nth_text_string(g_season_word[g_season]));
    append_comma_space(buf);
    LTOA1(g_year, num, 10);
    strcat(buf, num);
    append_period_spaces(buf);
    say_string(buf, g_word_d994);
    say_number(buf, g_cur_nation_rec->tax);
    append_percent(buf);
    append_space(buf);
    append_space(buf);
    say_string(buf, g_word_d984);
    append_space(buf);
    say_nation_gold(g_cur_nation, buf);
    sub_1048_d48b(w, buf);
}
