/* PORT: this file's own declarations of Sprite_Init removed: the header declares it from the definition (W1 step 3). */
/* init_letter_sprites: candidate for 1060:7f33 (78 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 78 -- the census is RIGHT here and the gap to the next entry is not. The 49
 * bytes between them are the exception descriptor table for the function that follows, which
 * the linker places before it.
 *
 * Initialises one sprite at +0x8bc from resource 1000, then TWENTY-SIX sprites of 32 bytes
 * each from +0x8dc, from resources 2000 through 2025.
 *
 * Twenty-six consecutive sprite resources is an alphabet, which is where the name comes
 * from; nothing in the function says so and the resource ids are not in `re/strings.json`.
 * The single sprite ahead of them is presumably the background the letters are drawn on.
 *
 * The stride is a shift (`SHL AX,5`) because 32 is a power of two, where the record arrays
 * elsewhere in the game get an `IMUL`.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void init_letter_sprites(AboutWin *o)
{
    register int i;

    Sprite_Init((Sprite *)&o->logo, 0x3e8);
    for (i = 0; i < 0x1a; i++)
        Sprite_Init((Sprite *)&o->letters[i], i + 0x7d0);
}
