/* rect_width_1068_99a9: candidate for 1068:99a9 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 31, not the census 27. The trailing 4 bytes are the SECOND epilogue that
 * the `return` adds (rule 44), which Ghidra excludes because nothing reaches it.
 *
 * This module's one-line forwarder to `rect_width_1068_96a9`, named for the routine it reaches.
 *
 * The callee is reached by the four-byte `PUSH CS / CALL NEAR` form, so it is in this
 * translation unit and appears here as a stub (rule 47).
 */

#include "colonization.h"

#include "w16types.h"

/* same object: 4-byte `push cs; call ` */

int rect_width_1068_99a9(void *sprite)
{
    return rect_width_1068_96a9((RECT *)sprite);
}
