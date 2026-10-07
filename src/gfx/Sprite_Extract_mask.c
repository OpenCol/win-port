/* Sprite_Extract_mask: candidate for 1068:782a (106 bytes).  Verdict: RELOCATABLE.
 *
 * Extracts a w x h sprite at (x, y) from the source port with an explicit transparent
 * colour byte: the handle goes to +0x1a and the sprite is locked. Both calls are calls
 * TLINK converted to its NOP-padded `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern short Sprite_ExtractSprite(Sprite *s, GRPort *port, int key, RECT *src);
extern void Sprite_Lock(Sprite *s);

void Sprite_Extract_mask(Sprite *s, void *src, unsigned char transparent, int x, int y, int w, int h)
{
    RECT rc;

    SetRect(&rc, x, y, x + w, y + h);
    s->handle = Sprite_ExtractSprite(s, (GRPort *)src, transparent, &rc);
    Sprite_Lock(s);
}
