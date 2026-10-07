/* port_draw_at: candidate for 1068:b144 (68 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_draw_at
 * name-source: descriptive
 *
 * One call with seven arguments and nothing else: the port at +0xc4, the object itself, the
 * rectangle at +0xee, the rectangle's first two words AGAIN by value, -1 and 10.
 *
 * Passing +0xee both by address and by value is what the shipped code does -- `LEA` for the
 * third argument and two `PUSH [es:bx+0xee]`/`[+0xf0]` for the fourth and fifth -- so the
 * callee is given the rectangle to write into and its current origin to read from, and the
 * caller does not trust it to do both through one pointer.
 *
 * The -1 is a word, not a `long`: `PUSH 0xffff` rather than one 32-bit push under `-3`.
 *
 * The arguments are evaluated right to left, which is why the two constants are pushed
 * before anything is read out of the object.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void DoFizzle(GRPort *port, GameWin *o, RECT *r, int x, int y,
                              int mode, int step);

void port_draw_at(MagicWin *o)
{
    DoFizzle(&o->port, (GameWin *)o, &o->rect, o->rect.left, o->rect.top, -1, 10);
}
