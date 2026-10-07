/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 17 of the imported file. */
/* PORT: this file's own declarations of sprite_load_palette_1068_6aac removed: the header declares it from the definition (W1 step 3). */
/* sprite_load_palette_1068_6a83: candidate for 1068:6a83 (41 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Convenience wrapper for `sprite_load_palette_1068_6aac`: use the palette range
 * stored in the SPRT resource by passing -1 for both override values.
 */

#include "colonization.h"

struct Sprite;
struct logpal;

void sprite_load_palette_1068_6a83(struct Sprite *s, int res_id,
                                       struct logpal *pal)
{
    sprite_load_palette_1068_6aac((Sprite *)(s), res_id, -1, -1, pal);
}
