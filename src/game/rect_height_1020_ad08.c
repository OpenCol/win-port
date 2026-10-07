/* rect_height_1020_ad08: candidate for 1020:ad08 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 31, not the census 27. The trailing 4 bytes are the SECOND epilogue that
 * the `return` adds (rule 44), which Ghidra excludes because nothing reaches it.
 *
 * This module's one-line forwarder to `rect_height_1020_b03e`, named for the routine it reaches.
 *
 * The callee is reached by the four-byte `PUSH CS / CALL NEAR` form, so it is in this
 * translation unit and appears here as a stub (rule 47).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */
#include "w16types.h"
#include "classes.h"

int rect_height_1020_ad08(Sprite *s)
{
    return rect_height_1020_b03e((RECT *)s);
}
