/* PORT: this file's own declarations of Sprite_ExtractSprite removed: the header declares it from the definition (W1 step 3). */
/* Sprite_Extract: candidate for 1068:7639 (102 bytes).  Verdict: RELOCATABLE.
 *
 * Extracts a w x h sprite at (x, y) from the source port with the default transparent
 * colour (-1): the handle goes to +0x1a and the sprite is locked. Both calls are calls
 * TLINK converted to its NOP-padded `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void Sprite_Lock(Sprite *s);

void Sprite_Extract(Sprite *spr, GRPort *src, int x, int y, int w, int h)
{
    RECT rc;

    SetRect(&rc, x, y, x + w, y + h);
    spr->handle = Sprite_ExtractSprite(spr, src, -1, &rc);
    Sprite_Lock(spr);
}
