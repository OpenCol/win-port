/* repaint_info_window -- shipped at 1058:b928.
 *
 * Ported by tools/port.py from matched/game/repaint_info_window.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void draw_info_window(void *w);

extern void get_scale_pair(col_i16 *a, col_i16 *b);
extern void scale_map_select(col_i16 a, col_i16 b);
extern void * current_window(void); /* 1060:9fcd, a call: it lives in SEG13 */



void repaint_info_window(void)
{
    void *p;
    col_i16 x, y;
    void *w;

    get_scale_pair(&x, &y);
    w = current_window();
    p = w != 0 ? (char *)w - 0x26 : (char *)w;
    draw_info_window(p);
    scale_map_select(x, y);
}
