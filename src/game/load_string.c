/* load_string -- shipped at 1040:7872.
 *
 * Ported by tools/port.py from matched/game/load_string.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_strbuf[];                          SEG29:0x10ea
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void pascal_to_c(char *s);

extern void load_string_resource(char *buf, col_i16 a, col_i16 n);



char * load_string(col_i16 a, col_i16 b)
{
    g_strbuf[0] = 0;
    load_string_resource(g_strbuf, a, b + 1);
    pascal_to_c(g_strbuf);
    return g_strbuf;
}
