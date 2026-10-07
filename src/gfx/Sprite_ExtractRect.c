/* Sprite_ExtractRect: candidate for 1068:75fa (63 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `Sprite_Extract` with the rectangle supplied instead of built: the same
 * `Sprite_ExtractSprite(s, src, -1, r)` with the same default transparent colour, the same handle
 * store at +0x1a and the same `Sprite_Lock`. Its twin 63 bytes below takes x, y, w, h and
 * calls `SetRect` first, which is the whole 39-byte difference between them.
 *
 * Both calls are calls TLINK converted to the NOP-padded form, so both stay `extern`
 * (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern short Sprite_ExtractSprite(Sprite *s, GRPort *port, int key, RECT *src);
extern void Sprite_Lock(Sprite *s);

void Sprite_ExtractRect(Sprite *s, void *src, RECT *r)
{
    s->handle = Sprite_ExtractSprite(s, (GRPort *)src, -1, r);
    Sprite_Lock(s);
}
