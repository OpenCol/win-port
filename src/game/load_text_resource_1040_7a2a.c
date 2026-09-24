/* load_text_resource_1040_7a2a -- shipped at 1040:7a2a.
 *
 * Ported by tools/port.py from matched/game/load_text_resource_1040_7a2a.c.
 *
 * STRLEN -> strlen, STRNCPY -> strncpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long g_res_key;                               DGROUP:0x1cff
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern void * memcpy_rtl(void *dst, const void *src, col_u16 n);
extern void * find_and_load_resource(col_i32 *key, char *name);
extern char * lock_resource(void *h);

extern void unlock_resource(void *h);
extern void free_resource(void *h);

col_i16 load_text_resource_1040_7a2a(char *name, char *dst, col_i16 n)
{
    void *h;
    char *p;
    col_i32 key = g_res_key;
    char buf[32];

    memcpy_rtl(buf, name, strlen(name) + 1);
    h = find_and_load_resource(&key, buf);
    if (h == 0)
        return 0;
    p = lock_resource(h);
    strncpy(dst, p, n);
    unlock_resource(h);
    free_resource(h);
    return 1;
}
