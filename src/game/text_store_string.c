/* text_store_string -- shipped at 1040:76f0.
 *
 * Ported by tools/port.py from matched/game/text_store_string.c.
 *
 * STRCPY -> strcpy, STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_text_pool[];                       SEG29:0x10dc
 *   extern int g_text_count;                             DGROUP:0x1cf4
 *   extern char far g_stub_s[];                          
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * heap_get(void *pool, col_i16 n);

col_i16 text_store_string(char *s)
{
    col_i16 n;
    char *p;

    n = strlen(s) + 1;
    p = heap_get(g_text_pool, n);
    strcpy(p, s);
    return g_text_count++;
}
