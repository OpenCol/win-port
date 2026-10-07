/* read_text_paragraph: candidate for 1040:620d (169 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * Reads lines (read_text_line) from a source into dst, each followed by a space, until the
 * reader fails, a line is empty, or the count reaches 511; lines beginning with '@' are
 * skipped. The count adds each line's length past its first character (strlen - 1), and
 * nothing for the appended space.
 *
 * RENAMED 2026-09-26 from read_words: Calls read_text_line (one CR/LF line per call) over and over. It appends each line plus a space to dst, skips lines that start with '@', and stops at the first empty line, at a read failure or at 511 characters. So it joins a run of file lines into one paragraph for DoFancyText. It does not read words.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern char read_text_line_1040_a89b(void *src, char *word, int max);

void read_text_paragraph(char *dst, void *src)
{
    char line[0x100];
    char ok;
    int len = 0;

    dst[0] = 0;
    ok = read_text_line_1040_a89b(src, line, 0xff);
    while (ok != 0 && line[0] != 0 && len < 0x1ff) {
        if ((int)line[0] != '@') {
            STRCAT(dst, line);
            STRCAT(dst, " ");
            len = STRLEN(&line[1]) + len;
        }
        ok = read_text_line_1040_a89b(src, line, 0xff);
    }
}
