/* load_text_resource -- shipped at 1040:6cf4.
 *
 * Ported by tools/port.py from matched/game/load_text_resource.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_resource_ctx2;                    DGROUP:0x1cf0
 *   extern void far *g_cur_res;                          DGROUP:0x4cb4
 *   extern void far *g_cur_data;                         DGROUP:0x4cb0
 *   extern void far *g_text_cursor_a, far *g_text_cursor_b; DGROUP:0x4cac, DGROUP:0x4ca8
 *   extern char far g_text[];                            SEG29:0x00dc
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * lock_resource_1040_a99c(void *res);
void * resource_find_1040_a976(void * *ctx, char *name);
void empty_1040_6ce6(void);
void free_resource_1040_a9d8(void *res);
void unlock_resource_1040_a9bc(void *res);

extern col_i32 block_size(void *res);
extern void MSCopyMem_1068_9424(void *src, void *dst, col_i32 n);


/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 load_text_resource(void *unused, char *name)
{
    col_i16 flag = 1;
    col_i16 loaded = 0;
    void *ctx = g_resource_ctx2;
    char buf[0x20];

    if (name == 0)
        flag = 0;
    else {
        strcpy(buf, name);
        g_cur_res = resource_find_1040_a976(&ctx, buf);
        if (g_cur_res == 0)
            return 0;
        g_cur_data = lock_resource_1040_a99c(g_cur_res);
        if (g_cur_data == 0)
            return 0;
        loaded = 1;
        MSCopyMem_1068_9424(g_cur_data, g_text, block_size(g_cur_res));
        g_text[block_size(g_cur_res)] = 0;
        g_text_cursor_b = g_text_cursor_a = g_cur_data = g_text;
        unlock_resource_1040_a9bc(g_cur_res);
        free_resource_1040_a9d8(g_cur_res);
        flag = 0;
    }
    if (flag)
        empty_1040_6ce6();
    return flag;
}
