/* sprite_height_1060_69bc: candidate for 1060:69bc (35 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 35, not the census 31. The trailing 4 bytes are the dead second epilogue.
 *
 * `rect_height_1060_63d4` on the member 0x8 bytes into the object -- the address is computed, not loaded,
 * so the member is embedded and not a pointer to one.
 *
 * RENAMED 2026-09-26 from rect_height_at_8_1060_69bc: the height of the sprite's RECT area at +8 (Sprite_Init's layout); every
 * caller passes a sprite -- g_icon_sprites entries, g_background_tile, a window's logo or tile.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */

int sprite_height_1060_69bc(Sprite *sprite)
{
    return rect_height_1060_63d4(&sprite->area);
}
