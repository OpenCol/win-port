/* rect_height_1058_bd4c: candidate for 1058:bd4c (32 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 32, not the census 28. The trailing 4 bytes are the dead second epilogue
 * the written-out `return` adds (rule 44).
 *
 * The height of a rectangle, bottom minus top -- this segment's copy. One small function compiled into
 * SEVENTEEN modules; both halves are loaded through their own `LES BX`, because `-Od` keeps
 * nothing.
 */

#include "colonization.h"

#include "w16types.h"
int rect_height_1058_bd4c(RECT *r)
{
    return r->bottom - r->top;
}
