/* FillColor -- shipped at 1068:505c.
 *
 * Ported by tools/port.py from matched/gfx/FillColor.c.
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
col_i16 rect_bottom(GRPort *p);
col_i16 rect_height_1068_96c8(RECT *r);
col_i16 rect_right(GRPort *p);
col_i16 rect_width_1068_96a9(RECT *r);
void * GRPort_GetWin_1068_968c(GRPort *p);
void MSLockGraf(GRPort *p);
void empty_1068_963d(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal IntersectRect(RECT *dst, RECT *a, RECT *b);
#endif

/* +0x18: the pixel buffer as a 32-bit value */
extern char GRPort_IsLocked(GRPort *p);
/* same object: 4-byte `push cs; call ` (rule 47) */









extern col_i16 is_state_one(void *o);
extern void sub_1090_01b4(col_u32 buf, col_i16 color, col_i16 left, col_i16 top, col_i16 w, col_i16 h, col_i16 pitch, col_i16 planes);

void FillColor(GRPort *p, RECT *r, col_i16 color)
{
    col_i16 w;
    col_i16 h;
    RECT clip;
    col_i16 locked = 1;

    if (!GRPort_IsLocked(p)) {
        empty_1068_963d(&(*g_error_ctx), "Warning: Port not locked in FillColor");
        locked = 0;
        MSLockGraf(p);
    }
    IntersectRect(&clip, r, &(*((RECT *)((unsigned char *)(p) + 0x8))));
    if ((w = rect_width_1068_96a9(&clip)) == 0 || (h = rect_height_1068_96c8(&clip)) == 0)
        return;
    sub_1090_01b4((*((col_u32 *)((unsigned char *)(p) + 0x18))), color, clip.left, clip.top, w, h,
                  rect_bottom(p) * (is_state_one(GRPort_GetWin_1068_968c(p)) ? 1 : -1), rect_right(p));
}
