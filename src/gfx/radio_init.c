/* radio_init -- shipped at 1068:df8f.
 *
 * Ported by tools/port.py from matched/gfx/radio_init.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void dlg_control_init_1068_e478(Ctl *c, void *win, col_i16 kind, col_i16 id, RECT *r);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_i16 MSInitRadio(void *sub, RECT *r, col_i16 n, Ctl *c, col_i16 a, col_i16 b, col_i16 f);
extern void * MSHLock(col_i16 h);

void radio_init(Ctl *c, void *win, col_i16 id, RECT *r,
                              register col_i16 n, col_i16 a, col_i16 b)
{
    dlg_control_init_1068_e478(c, win, 3, id, r);
    (*((col_i32 *)((unsigned char *)(c) + 0x1a))) = 0;
    (*((col_i32 *)((unsigned char *)(c) + 0x1e))) = 0;
    (*((col_i32 *)((unsigned char *)(c) + 0x22))) = 0;
    (*((col_i16 *)((unsigned char *)(c) + 0x28))) = n;
    (*((col_i16 *)((unsigned char *)(c) + 0x2a))) = 0;
    (*((col_i16 *)((unsigned char *)(c) + 0x2c))) = MSInitRadio((char *)c + 0x26, r, n, c, a, b, 1);
    (*((void * *)((unsigned char *)(c) + 0x2e))) = MSHLock((*((col_i16 *)((unsigned char *)(c) + 0x2c))));
}
