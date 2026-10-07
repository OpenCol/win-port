/* rect_height: candidate for 1070:dc24 (32 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 32, not the census 28. The trailing 4 bytes are the dead second epilogue
 * the written-out `return` adds (rule 44).
 *
 * The height of a rectangle, bottom minus top -- this segment's copy. One small function compiled into
 * SEVENTEEN modules; both halves are loaded through their own `LES BX`, because `-Od` keeps
 * nothing.
 *
 * TRANSFERRED: byte-identical to `rect_height_1008_d5ee` (1008:d5ee), one of 19 copies of this
 * function in the image. Same source, this segment's copy.
 */

#include "colonization.h"

#include "w16types.h"
int rect_height(RECT *r)
{
    return r->bottom - r->top;
}
