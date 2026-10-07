/* sprite_width_1068_9abd: candidate for 1068:9abd (35 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 35, not the census 31. The trailing 4 bytes are the dead second epilogue.
 *
 * `rect_width_1068_96a9` on the member 0x8 bytes into the object -- the address is computed, not loaded,
 * so the member is embedded and not a pointer to one.
 *
 * RENAMED 2026-09-26 from rect_width_at_8_1068_9abd: the width of the sprite's RECT area at +8 (Sprite_Init's layout); every
 * caller passes a sprite -- g_icon_sprites entries, g_background_tile, a window's logo or tile.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */

int sprite_width_1068_9abd(Sprite *sprite)
{
    return rect_width_1068_96a9(&sprite->area);
}
