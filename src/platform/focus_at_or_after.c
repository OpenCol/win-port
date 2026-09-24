/* focus_at_or_after -- shipped at 1070:928c.
 *
 * Ported by tools/port.py from matched/platform/focus_at_or_after.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char byte_at_98(void *o);
col_i16 * get_p_2e_1070_99c5(void *o);
col_i16 get_w_0_1070_9669(void *o);
col_i16 get_w_10_1070_9681(void *o);
col_i16 prev_sibling_window(col_i16 h);
void focus_walk_siblings(void *o, col_i16 h, col_i16 n);

/* same object: 4-byte `push cs; call ` (rule 47) */







void focus_at_or_after(void *o, void *w)
{
    void *p;
    col_i16 *q;
    register col_i16 h;

    if (byte_at_98(o) == 0)
        return;
    if (get_w_0_1070_9669(w) == 3) {
        p = w;
        q = get_p_2e_1070_99c5(p);
        h = *q;
        h = prev_sibling_window(h);
        focus_walk_siblings(o, h, 2);
    } else {
        h = prev_sibling_window(get_w_10_1070_9681(w));
        focus_walk_siblings(o, h, 2);
    }
}
