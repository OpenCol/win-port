/* rect_width_1010_2b62: candidate for 1010:2b62 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 31, not the census 27. The trailing 4 bytes are the dead second epilogue.
 *
 * A one-line forwarder to `rect_width_1010_2ea8`, one of THREE the same size back to back at
 * 1010:2b62, 2b81 and 2ba0. The callee is reached by the four-byte `PUSH CS / CALL NEAR` form, so it is in
 * this translation unit and appears here as a stub (rule 47).
 */

#include "colonization.h"

#include "w16types.h"

/* same object: 4-byte `push cs; call ` */

#include "classes.h"

int rect_width_1010_2b62(Sprite *s)
{
    return rect_width_1010_2ea8(&s->bounds);
}
