/* Sprite_dtor: candidate for 1068:664b (116 bytes, 30 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Sprite::~Sprite()
 *
 * The destructor of Sprite: when the pixel handle at +0x1a is set, unlock it if its 
 * pointer at +0x1c is live, then dispose it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-

Sprite::~Sprite()
{
    if (handle != 0) {
        if (bits != 0)
            bits = (unsigned char *)MSHUnlock(handle);
        MSDisposeHandle(handle);
    }
}
