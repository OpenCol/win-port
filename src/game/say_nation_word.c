/* say_nation_word -- shipped at 1050:5e81.
 *
 * Ported by tools/port.py from matched/game/say_nation_word.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_nation3_prefix_word;                SEG20:0xdd10
 *   extern int far g_nation_name[];                      SEG20:0xd896
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void say_string(char *s, col_i16 word);
extern void append_space(char *s);
extern col_i16 tolower_rtl(col_i16 c);

void say_nation_word(col_i16 k, col_i16 flag, char *s)
{
    col_i16 n = k;

    if (n == 3) {
        say_string(s, g_nation3_prefix_word);
        append_space(s);
        if (flag == 0)
            *s = tolower_rtl(*s);
    }
    say_string(s, g_nation_name[n]);
}
