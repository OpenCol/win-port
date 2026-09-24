/* CopyToScreen_1068_558e -- shipped at 1068:558e.
 *
 * Ported by tools/port.py from matched/gfx/CopyToScreen_1068_558e.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * The file's own MSWin, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char GRPort_IsLocked(GRPort *p);
col_i16 rect_height_1068_96c8(void *r);
col_i16 rect_width_1068_96a9(void *r);
void * get_p_2_1068_966f(void *r);
void * get_p_6_1068_96e8(void *r);
void MSLockGraf(GRPort *p);
void MSUnlockGraf(GRPort *p);
void empty_1068_963d(void *ctx, char *msg);

/* surface at +0x18, window at +0x22 */

extern void port_select_if_changed(GRPort *p, void *page);
extern void wing_stretch(MSWin *w, col_i16 x, col_i16 y, col_i16 cw, col_i16 ch,
                                       void *src, col_i16 sx, col_i16 sy, col_i16 sw, col_i16 sh);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */





    /* the rectangle's width */
    /* the rectangle's height */


void CopyToScreen_1068_558e(GRPort *p, POINT *src, POINT *dst, POINT *at)
{
    col_i16 locked = 1;

    if (!GRPort_IsLocked(p)) {
        empty_1068_963d(&(*g_error_ctx), "Warning: Port not locked in CopyToScreen");
        locked = 0;
        MSLockGraf(p);
    }
    port_select_if_changed(p, get_p_2_1068_966f(src));
    wing_stretch((*((MSWin * *)((unsigned char *)(p) + 0x22))), dst->x, dst->y, rect_width_1068_96a9(dst), rect_height_1068_96c8(dst),
                           get_p_6_1068_96e8(src), at->x, at->y, rect_width_1068_96a9(at),
                           rect_height_1068_96c8(at));
    if (!locked)
        MSUnlockGraf(p);
}
