/* read_line -- shipped at 1050:5449.
 *
 * Ported by tools/port.py from matched/game/read_line.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_line[256];                             DGROUP:0x4cfe
 *   extern char far *g_cursor;                           DGROUP:0x2508
 *   extern char far *g_line_text;                        DGROUP:0x250c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char * read_line(char *text)
{
    col_i16 i;

    g_line[0] = 0;
    if (g_line_text != text) {
        g_cursor = text;
        g_line_text = text;
    } else if (g_line_text != 0 && g_cursor[-1] == 0)
        return g_line;
    if (g_line_text == 0)
        return 0;
    i = 0;
    while (i < 255 && (col_i16)*g_cursor != '\n' && *g_cursor != 0) {
        g_line[i] = *g_cursor;
        i++;
        g_cursor++;
    }
    g_line[i] = 0;
    g_cursor++;
    return g_line;
}
