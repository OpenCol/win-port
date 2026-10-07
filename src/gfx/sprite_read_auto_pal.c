/* PORT: this file's own declarations of sprite_read_file_pal removed: the header declares it from the definition (W1 step 3). */
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


void sprite_read_auto_pal(Sprite *sprite, char *name, void *pal)
{
    sprite_read_file_pal(sprite, name, -1, -1, pal);
}

