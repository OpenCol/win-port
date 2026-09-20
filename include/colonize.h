/* colonize.h -- every shared constant header at once.
 *
 * The shared headers name the numbers the game's code is written in: terrain ids and map-plane
 * bits, nation indices, unit types, goods, and the Windows API constants. They exist so a
 * reconstruction can say `t == TERRAIN_OCEAN` instead of `t == 25` and `WM_PAINT` instead of 0xf.
 *
 * THEY CANNOT CHANGE A BYTE, and that is a design rule, not luck:
 *   - every name is a #define, never an enum or a variable;
 *   - every value is spelled the way the matched code already spells it. In a 16-bit compiler the
 *     SPELLING of a literal sets its type -- 0x8000 is unsigned int, 32768 is long -- so a header
 *     that "tidied" a value could change a comparison;
 *   - nothing that affects codegen is shared: no prototypes, and no game record structures.
 *
 * Opting in is `#include "colonize.h"` (or one topic header) after the file's header comment.
 * Every compile path puts include/ on the include path; see include/README.md.
 *
 * w16types.h is NOT included here: its typedefs collide with a file's own RECT or POINT, so it
 * is included by name where wanted.
 */
#ifndef COLONIZE_H
#define COLONIZE_H

#include "terrain.h"
#include "nations.h"
#include "units.h"
#include "goods.h"
#include "fathers.h"
#include "difficul.h"
#include "jobs.h"
#include "sounds.h"
#include "w16const.h"

#endif
