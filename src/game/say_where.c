/* say_where -- shipped at 1058:923d.
 *
 * Ported by tools/port.py from matched/game/say_where.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern int far g_nation_name_word[];                 SEG20:0xd8ae
 *   extern int far g_europe_word;                        SEG20:0xdd40
 *   extern int far g_cur_colony_idx;                     DGROUP:0x4ba2
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

#include "terrain.h"
/* 0xca bytes at SEG20:0x7c28 */




extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern col_i16 colony_at(col_i16 x, col_i16 y);
extern void say_string(char *buf, col_i16 word);
extern void say_number(char *s, col_i16 v);
extern void append_lparen(char *s);
extern void append_rparen(char *s);
extern void append_comma_space(char *s);


void say_where(char *buf, col_i16 x, col_i16 y, col_i16 nation, col_i16 flag)
{
    if (!on_map(x, y)) {
        if (nation >= 0 && (char)(nation - 20) == (char)x)
            say_string(buf, g_nation_name_word[nation]);
        else
            say_string(buf, g_europe_word);
    } else if (terrain_at(x, y) == TERRAIN_SEA_LANE && flag != 0) {
        say_string(buf, g_nation_name_word[nation]);
    } else {
        g_cur_colony_idx = colony_at(x, y);
        if (g_cur_colony_idx >= 0)
            strcpy(buf, ((char *)((unsigned char *)&(g_colonies[g_cur_colony_idx]) + 0x2)));
        else {
            append_lparen(buf);
            say_number(buf, x);
            append_comma_space(buf);
            say_number(buf, y);
            append_rparen(buf);
        }
    }
}
