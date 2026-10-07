/* sprite_read_auto_pal: candidate for 1068:7136 (44 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: sprite_read_auto_pal
 * name-source: descriptive
 *
 * `sprite_read_file_pal` with both range arguments at -1, which is what tells it to
 * install the palette triples at the first index and count the file itself carries.
 */

#include "colonization.h"

class Sprite;

extern void sprite_read_file_pal(Sprite *s, char *name, int first,
                                               int count, void *pal);

void sprite_read_auto_pal(Sprite *sprite, char *name, void *pal)
{
    sprite_read_file_pal(sprite, name, -1, -1, pal);
}

