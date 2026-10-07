/* get_ctable_1068_d77f: candidate for 1068:d77f (29 bytes, 12 callers).  Verdict: EXACT.
 *
 * Returns the pointer at +2 of a window: its colour table (see below).
 *
 * RENAMED 2026-09-26 from port_bits: the window's colour table, the pointer at +2 -- another copy of get_ctable,
 * not pixel storage as this was first read: the sprite loaders and port_blit_and_select hand it to
 * GRPort_ColorSync and compare it with the sprite's palette, port_fade_through_black keeps it as `tbl`,
 * and PostLowMemReport gives it to build_window_with_port as the new window's table.
 */

#include "colonization.h"

#include "classes.h"

void *get_ctable_1068_d77f(Screen *p)
{
    return p->ctable;
}
