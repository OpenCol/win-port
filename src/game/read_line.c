/* read_line: candidate for 1050:5449 (168 bytes, 2 callers).  Verdict: RELOCATABLE.
 *
 * Reads the next line of a text into a static 256-byte buffer. Passing a different
 * text resets the cursor to its start; passing the same text again continues from where
 * the last call stopped. Returns the buffer, or NULL when there is no text, and the
 * buffer emptied (its first byte is cleared on entry) when the previous call consumed
 * the terminating NUL.
 *
 * This function found the flag set. With -Ot alone Borland merges the early `return
 * g_line` into the final identical one as a 2-byte jump; the shipped code has both
 * inline, and only `-Od -Ot` (all optimisation off, then speed on) reproduces that --
 * without disturbing any earlier match. The newline test is `(int)*g_cursor != '\n'`:
 * the shipped code promotes with CBW where a plain char compare is a byte CMP.
 */

#include "colonization.h"

char * read_line(char *text)
{
    int i;

    g_line[0] = 0;
    if (g_line_text != text) {
        g_cursor = text;
        g_line_text = text;
    } else if (g_line_text != 0 && g_cursor[-1] == 0)
        return g_line;
    if (g_line_text == 0)
        return 0;
    i = 0;
    while (i < 255 && (int)*g_cursor != '\n' && *g_cursor != 0) {
        g_line[i] = *g_cursor;
        i++;
        g_cursor++;
    }
    g_line[i] = 0;
    g_cursor++;
    return g_line;
}
