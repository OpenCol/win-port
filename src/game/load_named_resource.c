/* load_named_resource -- shipped at 1040:66e0.
 *
 * Ported by tools/port.py from matched/game/load_named_resource.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_resource_ctx;                     DGROUP:0x1ce8
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * lock_resource_1040_a99c(void *res);
void * resource_find_1040_a976(void * *ctx, char *name);
void free_resource_1040_a9d8(void *res);
void text_prompt(void *target, void *arg, void *data);
void unlock_resource_1040_a9bc(void *res);

/* same object: 4-byte `push cs; call ` (rule 47) */



void load_named_resource(void *target, char *name, void *arg)
{
    void *ctx = g_resource_ctx;
    void *res;
    void *data;
    char buf[0x20];

    strcpy(buf, name);
    res = resource_find_1040_a976(&ctx, buf);
    data = lock_resource_1040_a99c(res);
    if (data != 0)
        text_prompt(target, arg, data);
    unlock_resource_1040_a9bc(res);
    free_resource_1040_a9d8(res);
}
