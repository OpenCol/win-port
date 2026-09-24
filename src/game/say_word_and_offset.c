/* say_word_and_offset -- shipped at 1020:9972.
 *
 * Ported by tools/port.py from matched/game/say_word_and_offset.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_founding_word[];                    SEG20:0xd790
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void say_string(char *buf, col_i16 word);
extern void append_space(char *s);
extern void say_number(char *s, col_i16 v);

void say_word_and_offset(col_i16 v, char *s)
{
    col_i16 t = v - 24;

    s[0] = 0;
    say_string(s, g_founding_word[5]);
    append_space(s);
    say_number(s, t);
}
