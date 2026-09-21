/* say_word_kind -- shipped at 1040:a628.
 *
 * Ported by tools/port.py from matched/game/say_word_kind.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern KindRec far g_terrain_rows[];                 SEG20:0x78f8
 *   extern int far g_default_word;                       SEG20:0xdd18
 *   extern int far g_feature_words;                      SEG20:0xd8d8
 *
 * The file's own KindRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void append_space(char *s);
void say_string(char *s, col_i16 word);

/* same object: 4-byte `push cs; call ` (rule 47) */

void say_word_kind(char *s, col_i16 k)
{
    col_i16 n = k;

    if (n >= 0)
        say_string(s, g_terrain_rows[n].word);
    else
        say_string(s, g_default_word);
    if (n >= 8 && n < 0x18) {
        append_space(s);
        say_string(s, (*g_feature_words));
    }
}
