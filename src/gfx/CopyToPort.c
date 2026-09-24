/* CopyToPort -- shipped at 1068:52d0.
 *
 * Ported by tools/port.py from matched/gfx/CopyToPort.c.
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
col_i16 min_int_1068_9847(col_i16 a, col_i16 b);
col_i16 rect_bottom(GRPort *p);
col_i16 rect_right(GRPort *p);
void * GRPort_GetWin_1068_968c(GRPort *p);
void * get_p_18_1068_989f(GRPort *p);
void MSLockGraf(GRPort *p);
void MSUnlockGraf(GRPort *p);
void empty_1068_963d(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal IntersectRect(RECT *dst, RECT *a, RECT *b);
#endif

/* same object: 4-byte `push cs; call ` (rule 47) */







extern col_i16 is_state_one(void *o);

extern void sub_1090_00ff(void *sbuf, void *dbuf, col_i16 sx, col_i16 sy, col_i16 dx, col_i16 dy, col_i16 w, col_i16 h, col_i16 splanes, col_i16 dplanes, col_i16 spitch, col_i16 dpitch);

void CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr)
{
    col_i16 w;
    col_i16 h;
    RECT a;
    RECT b;
    col_i16 src_locked = 1;
    col_i16 dst_locked = 1;

    if ((*((void * *)((unsigned char *)(src) + 0x18))) == 0) {
        empty_1068_963d(&(*g_error_ctx), "Warning: Source Port not locked in CopyToPort");
        MSLockGraf(src);
        src_locked = 0;
    }
    if ((*((void * *)((unsigned char *)(dst) + 0x18))) == 0) {
        empty_1068_963d(&(*g_error_ctx), "Warning: Destination Port not locked in CopyToPort");
        MSLockGraf(dst);
        dst_locked = 0;
    }
    IntersectRect(&a, sr, &(*((RECT *)((unsigned char *)(src) + 0x8))));
    IntersectRect(&b, dr, &(*((RECT *)((unsigned char *)(dst) + 0x8))));
    w = min_int_1068_9847(b.right - b.left, a.right - a.left);
    h = min_int_1068_9847(b.bottom - b.top, a.bottom - a.top);
    if (w == 0 || h == 0)
        goto done;
    sub_1090_00ff(get_p_18_1068_989f(src), get_p_18_1068_989f(dst), a.left, a.top, b.left, b.top, w, h,
                  rect_right(src), rect_right(dst),
                  rect_bottom(src) * (is_state_one(GRPort_GetWin_1068_968c(src)) ? 1 : -1),
                  rect_bottom(dst) * (is_state_one(GRPort_GetWin_1068_968c(dst)) ? 1 : -1));
done:
    if (!src_locked)
        MSUnlockGraf(src);
    if (!dst_locked)
        MSUnlockGraf(dst);
}
