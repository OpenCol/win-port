/* Sprite_ctor: candidate for 1068:6608 (67 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Sprite::Sprite()
 *
 * The constructor of Sprite (0x20 bytes): hands the object to Sprite_InitStruct.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
extern void Sprite_InitStruct(Sprite *s);

Sprite::Sprite()
{
    Sprite_InitStruct(this);
}
