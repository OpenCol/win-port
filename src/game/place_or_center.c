/* place_or_center -- shipped at 1048:0520.
 *
 * Ported by tools/port.py from matched/game/place_or_center.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_2_1048_d4f6(Win *w);
col_i16 get_w_4_1048_d7c5(Win *w);
col_i16 rect_height_1048_d6ba(RECT *r);
col_i16 rect_width_1048_d69b(RECT *r);
void win_center_1048_d80b(char *w);
void win_move_to_1048_d7e4(char *w, col_i16 x, col_i16 y);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void MSGetDesktopSize(RECT *r);


/* same object: 4-byte `push cs; call ` (rule 47) */







void place_or_center(Win *w)
{
    RECT r;

    MSGetDesktopSize(&r);
    if (rect_width_1048_d69b(&r) == 0x280
            && (get_w_2_1048_d4f6(w) > 0x280 || get_w_4_1048_d7c5(w) > 0x1e0)) {
        if (rect_height_1048_d6ba(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) <= 0x1ca) {
            if (rect_height_1048_d6ba(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) > 0x1c0) {
                SetRect(&r, -(*((RECT *)((unsigned char *)(w) + 0x76a))).left, -10,
                        get_w_2_1048_d4f6(w) - (*((RECT *)((unsigned char *)(w) + 0x76a))).left,
                        get_w_4_1048_d7c5(w) - 10);
            } else {
                SetRect(&r, -(*((RECT *)((unsigned char *)(w) + 0x76a))).left, 0,
                        get_w_2_1048_d4f6(w) - (*((RECT *)((unsigned char *)(w) + 0x76a))).left,
                        get_w_4_1048_d7c5(w));
            }
        } else {
            SetRect(&r, -(*((RECT *)((unsigned char *)(w) + 0x76a))).left, -(*((RECT *)((unsigned char *)(w) + 0x76a))).top,
                    get_w_2_1048_d4f6(w) - (*((RECT *)((unsigned char *)(w) + 0x76a))).left,
                    get_w_4_1048_d7c5(w) - (*((RECT *)((unsigned char *)(w) + 0x76a))).top);
        }
        win_move_to_1048_d7e4((char *)w + 0x26, r.left, r.top);
    } else {
        win_center_1048_d80b((char *)w + 0x26);
    }
}
