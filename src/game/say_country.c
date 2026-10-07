/* say_country: candidate for 1050:5e81 (118 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * For nation 3 first a leading word, a space, and unless `flag` the first letter lowered;
 * then the nation's word from a per-nation table.
 *
 * RENAMED 2026-09-27 from say_nation_word: the Mac build's say_country(short, short, char*), bound by LAYOUT (tools/mac_bind.py, 1.0 to 0, arity 3, body ratio 1.08) once 1050:5c00 stopped holding the name: it appends a nation's name to a buffer, with a leading "The" for nation 3.
 */

#include "colonization.h"

#include "misc.h"

extern void space_string(char *buf);

void say_country(int nation, int flag, char *s)
{
    int n = nation;

    if (n == 3) {
        say_string(s, g_misc_words[MISC_THE]);
        space_string(s);
        if (flag == 0)
            *s = tolower_rtl(*s);
    }
    say_string(s, g_nation_name[n]);
}
