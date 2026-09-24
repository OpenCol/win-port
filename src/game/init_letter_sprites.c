/* init_letter_sprites -- shipped at 1060:7f33.
 *
 * Ported by tools/port.py from matched/game/init_letter_sprites.c.
 *
 * The file's own Obj dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void Sprite_Init(void *s, col_i16 res);

void init_letter_sprites(Obj *o)
{
    register col_i16 i;

    Sprite_Init(((char *)((unsigned char *)(o) + 0x8bc)), 0x3e8);
    for (i = 0; i < 0x1a; i++)
        Sprite_Init(((char *)((unsigned char *)(o) + 0x8dc))[i], i + 0x7d0);
}
