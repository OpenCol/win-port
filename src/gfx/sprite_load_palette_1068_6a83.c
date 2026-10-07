/* sprite_load_palette_1068_6a83: candidate for 1068:6a83 (41 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Convenience wrapper for `sprite_load_palette_1068_6aac`: use the palette range
 * stored in the SPRT resource by passing -1 for both override values.
 */

#include "colonization.h"

struct Sprite;
struct logpal;
extern void sprite_load_palette_1068_6aac(struct Sprite *s, int res,
                                               int start, int count,
                                               struct logpal *pal);

void sprite_load_palette_1068_6a83(struct Sprite *s, int res_id,
                                       struct logpal *pal)
{
    sprite_load_palette_1068_6aac(s, res_id, -1, -1, pal);
}
