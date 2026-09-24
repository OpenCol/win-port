/* InitCtrlBlock -- shipped at 1070:08e5.
 *
 * Ported by tools/port.py from matched/platform/InitCtrlBlock.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * The file's own CtrlBlock dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: f1c -> kind
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1070_969a(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal SetWindowLong(col_u16 hwnd, col_i16 index, col_i32 value);
#endif

/* 0x1e bytes */
extern col_u16 MSAllocateHandle(col_u32 size);
extern CtrlBlock * MSHLock(col_i16 h);

/* same object: 4-byte `push cs; call ` (rule 47) */



CtrlBlock * InitCtrlBlock(col_u16 hwnd, void *obj)
{
    CtrlBlock *c = 0;
    col_u16 h;

    if (hwnd != 0) {
        h = MSAllocateHandle(0x1eL);
        c = MSHLock(h);
        (*((col_i16 *)((unsigned char *)(c) + 0x6))) = 0;
        (*((col_i16 *)((unsigned char *)(c) + 0x8))) = (*((col_i16 *)((unsigned char *)(c) + 0xa))) = (*((col_i16 *)((unsigned char *)(c) + 0xc))) = 0;
        (*((col_u16 *)((unsigned char *)(c) + 0x0))) = h;
        (*((void * *)((unsigned char *)(c) + 0x2))) = obj;
        (*((col_i16 *)((unsigned char *)(c) + 0xe))) = 0;
        (*((col_i16 *)((unsigned char *)(c) + 0x10))) = 0;
        (*((col_i16 *)((unsigned char *)(c) + 0x12))) = 0;
        c->kind = 0;
        SetWindowLong(hwnd, 0, (col_i32)c);
    } else {
        empty_1070_969a(&(*g_error_ctx), "Error: Tried to initialize NULL control in InitCtrlBlock");
    }
    return c;
}
