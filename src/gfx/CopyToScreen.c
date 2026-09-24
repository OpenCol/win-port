/* CopyToScreen -- shipped at 1068:54a3.
 *
 * Ported by tools/port.py from matched/gfx/CopyToScreen.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * The file's own GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1068_96c8(void *r);
col_i16 rect_width_1068_96a9(void *r);
void * get_p_2_1068_966f(void *r);
void * get_p_6_1068_96e8(void *r);
void MSLockGraf(GRPort *p);
void MSUnlockGraf(GRPort *p);
void empty_1068_963d(void *ctx, char *msg);

/* +0x22 is the WINDOW: MSLockGraf hands it to wing_dib_pointer and stores the surface it gets back at +0x18 */


extern char GRPort_IsLocked(GRPort *p);
/* same object: 4-byte `push cs; call ` (rule 47) */



extern void port_select_if_changed(GRPort *p, void *page);



extern void wing_blit(void *win, col_i16 x, col_i16 y, col_i16 w, col_i16 h, void *src, col_i16 dx, col_i16 dy);

void CopyToScreen(GRPort *p, POINT *src, POINT *dst, POINT *at)
{
    col_i16 locked = 1;

    if ((*((void * *)((unsigned char *)(p) + 0x22))) == 0) {
        empty_1068_963d(&(*g_error_ctx), "Error: NULL Port in CopyToScreen");
        return;
    }
    if (!GRPort_IsLocked(p)) {
        empty_1068_963d(&(*g_error_ctx), "Warning: Port not locked in CopyToScreen");
        locked = 0;
        MSLockGraf(p);
    }
    port_select_if_changed(p, get_p_2_1068_966f(src));
    wing_blit((*((void * *)((unsigned char *)(p) + 0x22))), dst->x, dst->y, rect_width_1068_96a9(dst), rect_height_1068_96c8(dst), get_p_6_1068_96e8(src), at->x, at->y);
    if (!locked)
        MSUnlockGraf(p);
}
