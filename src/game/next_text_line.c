/* next_text_line -- shipped at 1040:6e22.
 *
 * Ported by tools/port.py from matched/game/next_text_line.c.
 *
 * STRCHR -> strchr: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_text_buf[];                        SEG29:0x003c
 *   extern char far *g_text_cursor_a;                    DGROUP:0x4cac
 *   extern char far *g_text_cursor_b;                    DGROUP:0x4ca8
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * cut_at_newline(char *s);
void empty_1040_6ce6(void);
void trim_both(char *s);

/* same object: 4-byte `push cs; call ` (rule 47) */




char * next_text_line(void)
{
    char *r = 0;
    char *p;
    col_i16 i = 0;

    while ((col_i16)*g_text_cursor_a != '\n' && *g_text_cursor_a != 0 && i < 0x4f) {
        g_text_buf[i] = *g_text_cursor_a;
        g_text_cursor_a++;
        i++;
    }
    g_text_buf[i] = 0;
    cut_at_newline(g_text_buf);
    trim_both(g_text_buf);
    do {
        p = strchr(g_text_buf, '_');
        if (p != 0)
            *p = ' ';
    } while (p != 0);
    g_text_cursor_b = g_text_buf;
    r = g_text_buf;
    g_text_cursor_a++;
    if (r == 0)
        empty_1040_6ce6();
    return r;
}
