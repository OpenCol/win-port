/* clamp_rect_to_window -- shipped at 1038:a05f.
 *
 * Ported by tools/port.py from matched/game/clamp_rect_to_window.c.
 *
 * The file's own Rect, Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_height_1038_dfdc(Rect *r);
col_i16 rect_width_1038_dfbd(Rect *r);

/* same object: 4-byte `push cs; call ` (rule 47) */


void clamp_rect_to_window(Win *w, Rect *r)
{
    col_i16 win_w, win_h, x, y, r_w, r_h;

    win_w = rect_width_1038_dfbd(&(*((Rect *)((unsigned char *)(w) + 0x76a))));
    win_h = rect_height_1038_dfdc(&(*((Rect *)((unsigned char *)(w) + 0x76a))));
    r_w = rect_width_1038_dfbd(r);
    r_h = rect_height_1038_dfdc(r);
    x = r->x;
    y = r->y;
    if (x + r_w > win_w)
        r->right = win_w;
    if (y + r_h > win_h)
        r->bottom = win_h;
}
