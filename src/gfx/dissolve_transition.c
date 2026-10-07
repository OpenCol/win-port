/* dissolve_transition: candidate for 1068:af72 (466 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: dissolve_transition
 * name-source: descriptive
 *
 * THE DISSOLVE. The window's own pixels are replaced, one random pixel at a time, by the port
 * kept at +0xc4: `max(width, height)` pixels are copied per pass and the rectangle at +0xee is
 * blitted to the screen after each, for `min(width, height)` passes, with a task pump between.
 * Random pixels mean the same one can be copied twice and some may never be, so the port is
 * copied over wholesale at the end and blitted once more -- the dissolve is cosmetic, and the
 * final state does not depend on it.
 *
 * Both pixel maps are `` pointers and the address arithmetic is written
 * `p + ((long)ry * rowbytes + rx)`: ONE addition, with the offsets summed as a long first.
 * Without the parentheses C's left-to-right grouping gives two additions -- each with its
 * own `__AHSHIFT` normalization -- and the shipped has one.
 *
 * The inner loop's counter and the random x are two separate `register` locals, SI and DI in
 * declaration order: with only one, Borland promotes a stack local into the other register and
 * the frame comes out two bytes short (rule 53).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern char *GRPort_PixelMap(GRPort *port, int x, int y);
extern void MSLockGraf(GRPort *port);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void port_copy_rect(MagicWin *w);
/* same object: 4-byte `push cs; call ` */
extern "C" int port_stride(void *port);

extern "C" void copy_rect_to_screen_1068_d86d(MagicWin *w, RECT *r);

void dissolve_transition(MagicWin *w)
{
    int x0;
    int y0;
    int width;
    int height;
    int rb1;
    int rb2;
    int pass;
    int ry;
    char *dst;
    char *src;
    char *dp;
    char *sp;
    register int i;
    register int rx;

    width = rect_width_1068_d79c(&w->rect);
    height = rect_height_1068_d7bb(&w->rect);
    x0 = w->rect.left;
    y0 = w->rect.top;
    rb1 = port_stride(w);
    rb2 = port_stride(&w->port);
    MSLockGraf((GRPort *)w);
    MSLockGraf(&w->port);
    src = GRPort_PixelMap(&w->port, x0, y0);
    dst = GRPort_PixelMap((GRPort *)w, x0, y0);
    for (pass = 0; min_int_1068_d7f4(width, height) > pass; pass++) {
        for (i = 0; max_int_1068_d81a(width, height) > i; i++) {
            rx = rand_scaled(width);
            ry = rand_scaled(height);
            dp = dst + ((long)ry * rb1 + rx);
            sp = src + ((long)ry * rb2 + rx);
            *dp = *sp;
        }
        copy_rect_to_screen_1068_d86d(w, &w->rect);
        MSDoTask_1068_d906();
    }
    port_copy_rect(w);
    copy_rect_to_screen_1068_d86d(w, &w->rect);
}
