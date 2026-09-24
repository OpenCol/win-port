/* read_words -- shipped at 1040:620d.
 *
 * Ported by tools/port.py from matched/game/read_words.c.
 *
 * STRCAT -> strcat, STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_space[];                               DGROUP:0x1d5d
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char read_text_line_1040_a89b(void *src, char *word, col_i16 max);

/* same object: 4-byte `push cs; call ` (rule 47) */



void read_words(char *dst, void *src)
{
    char word[0x100];
    char ok;
    col_i16 len = 0;

    dst[0] = 0;
    ok = read_text_line_1040_a89b(src, word, 0xff);
    while (ok != 0 && word[0] != 0 && len < 0x1ff) {
        if ((col_i16)word[0] != '@') {
            strcat(dst, word);
            strcat(dst, g_space);
            len = strlen(&word[1]) + len;
        }
        ok = read_text_line_1040_a89b(src, word, 0xff);
    }
}
