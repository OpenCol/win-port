/* text_buf_seek_end -- shipped at 1040:6f95.
 *
 * Ported by tools/port.py from matched/game/text_buf_seek_end.c.
 *
 * STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_text_buf[];                        SEG29:0x003c
 *   extern char far *g_text_cursor;                      DGROUP:0x4ca8
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void text_buf_seek_end(void)
{
    g_text_cursor = g_text_buf + strlen(g_text_buf);
}
