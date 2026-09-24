/* GRPort_ChangeColor -- shipped at 1068:51a8.
 *
 * Ported by tools/port.py from matched/gfx/GRPort_ChangeColor.c.
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
char GRPort_IsLocked(GRPort *p);
col_i16 rect_height_1068_96c8(RECT *r);
col_i16 rect_width_1068_96a9(RECT *r);
void MSLockGraf(GRPort *p);
void MSUnlockGraf(GRPort *p);
void empty_1068_963d(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal IntersectRect(RECT *d, const RECT *a, const RECT *b);
#endif

/* the live rectangle at +8 */


extern unsigned char * GRPort_PixelMap(GRPort *p, col_i16 x, col_i16 y);
extern unsigned char * port_row_step(GRPort *p, unsigned char *q);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */




    /* the rectangle's width */
    /* the rectangle's height */

void GRPort_ChangeColor(GRPort *p, RECT *clip,
                                      unsigned char from, unsigned char to)
{
    col_i16 w;
    unsigned char *q;
    RECT r;
    col_i16 locked;
    register col_i16 x;
    register col_i16 y;

    locked = 1;
    if (!GRPort_IsLocked(p)) {
        empty_1068_963d(&(*g_error_ctx), "Warning: Port not locked in ChangeColor");
        locked = 0;
        MSLockGraf(p);
    }
    IntersectRect(&r, clip, &(*((RECT *)((unsigned char *)(p) + 0x8))));
    w = rect_width_1068_96a9(&r);
    q = GRPort_PixelMap(p, r.left, r.top);
    for (y = 0; rect_height_1068_96c8(&r) > y; y++) {
        for (x = 0; rect_width_1068_96a9(&r) > x; x++) {
            if (*q == from)
                *q = to;
            q++;
        }
        q = port_row_step(p, q) - w;
    }
    if (locked == 0)
        MSUnlockGraf(p);
}
