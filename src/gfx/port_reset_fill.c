/* PORT: this file's own declarations of port_bounds removed: the header declares it from the definition (W1 step 3). */
/* port_reset_fill: candidate for 1068:aa47 (72 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_reset_fill
 * name-source: descriptive
 *
 * Resets the object's rectangle at +0xee to the port's own -- `port_bounds` fetches
 * the port's second rectangle, at +0x10 of the port at +0xc4 -- and fills the port with one
 * colour.
 *
 * The colour is a BYTE widened on the way in (`MOV AL,[bp+0xa] / MOV AH,0`), so the parameter
 * is `unsigned char` and `GRPort_FillColor_all` takes an `int`; the same pair
 * `Sprite_Init_wh` has.
 *
 * `CopyRect` is `` and takes destination first, which is why the object's rectangle is
 * pushed before the port's.
 *
 * `port_bounds` is reached by the four-byte form and is a stub here;
 * `GRPort_FillColor_all` by the five-byte NOP form and is `extern` (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_FillColor_all(GRPort *p, int colour);

/* same object: 4-byte `push cs; call ` */

void port_reset_fill(MagicWin *o, unsigned char colour)
{
    CopyRect(&o->rect, port_bounds(&o->port));
    GRPort_FillColor_all(&o->port, colour);
}
