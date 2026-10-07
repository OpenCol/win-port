/* port_height_1008_db11: candidate for 1008:db11 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 31, not the census 27. The trailing 4 bytes are the SECOND epilogue that
 * the `return` adds (rule 44), which Ghidra excludes because nothing reaches it.
 *
 * This module's one-line forwarder to `port_height_1008_d720`, named for the routine it reaches.
 *
 * RETURN TYPE CORRECTED. It was `void *`, which the comparator cannot distinguish from
 * `int` -- both come back in DX:AX and a pure forwarder loads neither. The callee reads one
 * WORD at +0x4 and returns it in AX alone, so the value is an `int`; one of the 20
 * forwarders of this family carried the wrong type until its own caller divided the result
 * by two with `CWD`, which would clobber a pointer's high half (§119, and see
 * `port_width_1060_6886`).
 *
 * The callee is reached by the four-byte `PUSH CS / CALL NEAR` form, so it is in this
 * translation unit and appears here as a stub (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */
extern int port_height_1008_d720(GRPort *port);

int port_height_1008_db11(GRPort *p)
{
    return port_height_1008_d720(p);
}
