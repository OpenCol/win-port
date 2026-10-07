/* rect_width_1040_a903: candidate for 1040:a903 (31 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 31, not the census 27. The trailing 4 bytes are the dead second epilogue
 * the written-out `return` adds (rule 44).
 *
 * The width of a rectangle, right minus left -- this segment's copy. One small function compiled into
 * SEVENTEEN modules; both halves are loaded through their own `LES BX`, because `-Od` keeps
 * nothing.
 */

#include "colonization.h"

#include "w16types.h"
int rect_width_1040_a903(RECT *rect)
{
    return rect->right - rect->left;
}
