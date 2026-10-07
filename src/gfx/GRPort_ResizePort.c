/* GRPort_ResizePort: candidate for 1068:4eae (37 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort::ResizePort(RECT*)
 * name-source: sibling-build
 * size-source: 37. The last 8 are a second, unreachable epilogue.
 *
 * Hands its two pointers to `GRPort_Init` and returns what that gives -- a port and the
 * rectangle to build it over. `GRPort_Init` is reached by the four-byte `push cs / call `,
 * so it is defined here as a stub (rule 47).
 *
 * RENAMED 2026-09-26 from grport_init_1068_4eae: the Mac build's GRPort::ResizePort(Rect*), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between GRPort_GetClip and GRPort_ColorSync, where the Mac gap holds 5 functions and the Windows one 12, so the pairing inside it is by size and arity (the Mac body is 1.35x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 *
 * A METHOD since 2026-10-02: GRPort::ResizePort, the virtual GRPort's vtable names.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int GRPort_Init(GRPort *p, RECT *r);

int GRPort::ResizePort(RECT *r)
{
    return GRPort_Init(this, r);
}
