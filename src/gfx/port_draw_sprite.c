/* port_draw_sprite: candidate for 1068:aafb (54 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_draw_sprite
 * name-source: descriptive
 *
 * `Sprite_Draw(&o->rect, rec, o->port, x, y)`: draws a sprite into the object's own port at
 * +0xc4 and records where it landed in the rectangle at +0xee. `port_blit_and_select`
 * (immediately before it) uses the same two fields the same way -- "records the destination
 * rectangle at +0xee, blits into the port at +0xc4" -- and this is its sprite counterpart.
 *
 * The call has a NOP before its `push cs`, the five-byte form TLINK leaves for an `extern`,
 * so the callee is declared here and not defined (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, MagicWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-








   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void port_draw_sprite(MagicWin *o, void *rec, int x, int y)
{
    Sprite_Draw(&o->rect, (Sprite *)rec, &o->port, x, y);
}
