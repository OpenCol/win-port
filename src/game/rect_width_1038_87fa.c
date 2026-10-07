/* rect_width_1038_87fa: candidate for 1038:87fa (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 31, not the census 27. The trailing 4 bytes are the SECOND epilogue that
 * the `return` adds (rule 44), which Ghidra excludes because nothing reaches it.
 *
 * This module's one-line forwarder to `rect_width_1038_8967`, named for the routine it reaches.
 * Its argument is a Sprite (declaration_of_independence passes its letter sprites), and the
 * rectangle it measures is the sprite's first member, `bounds`.
 *
 * The callee is reached by the four-byte `PUSH CS / CALL NEAR` form, so it is in this
 * translation unit and appears here as a stub (rule 47).
 */

#include "colonization.h"

#include "w16types.h"

/* same object: 4-byte `push cs; call ` */

#include "classes.h"

int rect_width_1038_87fa(Sprite *s)
{
    return rect_width_1038_8967(&s->bounds);
}
