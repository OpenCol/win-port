/* text_get: candidate for 1040:6e22 (218 bytes, 60 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Copies the next line of the loaded text (up to 79 characters) into the shared line
 * buffer, cuts at the newline, trims, turns every '_' into a space, points the field
 * cursor at it, advances the text cursor, and returns the buffer.
 *
 * THE TWO CURSORS ARE DIFFERENT OBJECTS and were called `g_text_cursor_a` and
 * `g_text_cursor_b`, which read as one pair and are not:
 *
 *   DGROUP:0x4cac  g_text_read_ptr  walks the LOADED RESOURCE, one character at a time
 *                                   until '\n'; `text_open` sets it to the start
 *   DGROUP:0x4ca8  g_text_cursor    points into g_text_buf, the line just copied, and is
 *                                   what `text_item_get` advances field by field
 *
 * The `_b` also hid a duplicate: 0x4ca8 was ALSO declared `g_text_cursor` in
 * `text_buf_seek_end`, and `check_global_names.py` discounted `_b` as a byte view of the
 * same word (rule 9's suffix vocabulary) rather than seeing a second name for one object.
 *
 * RENAMED 2026-09-26 from next_text_line: the Mac build's text_get(void), bound by call-sequence alignment: in text_search the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (0), and the body does what the Mac name says.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void fileio_purge_all_spaces(char *s);

extern void text_close(void);
char * text_get(void)
{
    char *r = 0;
    char *p;
    int i = 0;

    while ((int)*g_text_read_ptr != '\n' && *g_text_read_ptr != 0 && i < 0x4f) {
        g_text_buf[i] = *g_text_read_ptr;
        g_text_read_ptr++;
        i++;
    }
    g_text_buf[i] = 0;
    cut_at_newline(g_text_buf);
    fileio_purge_all_spaces(g_text_buf);
    do {
        p = STRCHR(g_text_buf, '_');
        if (p != 0)
            *p = ' ';
    } while (p != 0);
    g_text_cursor = g_text_buf;
    r = g_text_buf;
    g_text_read_ptr++;
    if (r == 0)
        text_close();
    return r;
}
