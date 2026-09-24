/* read_next_text_value -- shipped at 1040:6fbd.
 *
 * Ported by tools/port.py from matched/game/read_next_text_value.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_text_buf[];                        SEG29:0x003c
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void next_text_line(void);

extern col_i16 text_store_string(char *s);



col_i16 read_next_text_value(void)
{
    col_i16 r;

    next_text_line();
    r = text_store_string(g_text_buf);
    return r;
}
