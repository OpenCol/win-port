/* MSLockGraf: candidate for 1068:4ed3 (317 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MSLockGraf
 *
 * Locks a graphics port for direct pixel access, and does nothing when it is already
 * locked. The window's DIB surface is fetched; if the window has one, a table of one
 * `long` per scanline is allocated and filled with the byte offset of each row. The row
 * order comes from the window: a bottom-up DIB (the Windows default, `MSGrafOrient`
 * false) starts the table at `(height - 1) * stride` and walks down by `stride`, and a
 * top-down surface starts at 0 and walks up. Drawing code then indexes rows without
 * caring which way the bitmap runs. `MSUnlockGraf` at the next address is the
 * other half and frees this table.
 *
 * The surface is a HUGE pointer, and the shipped bytes are what prove it: under `-3` a
 * `` pointer compared against 0 is one `cmp dword`, while a `` pointer is tested
 * as `mov ax,[off] / or ax,[seg]` -- the two halves, because a pointer may be
 * unnormalised and is not comparable as a flat 32-bit value. The shipped code uses the
 * two-half form in the same function that multiplies with `imul eax`, which only `-3`
 * emits; `` is the single reading under which both hold. A DIB wider than 64K needs
 * it anyway.
 *
 * The name is the Macintosh build's. `MSLockGraf(CGrafPort*)` and
 * `MSUnlockGraf(CGrafPort*)` are both in macui, both take one pointer, and this build
 * already spells its graphics layer `MSDrawString_rect`, `MSInitButton`, `MSGetWinSize`. The
 * 22 call sites -- `GRPort_CopyToPort`, `CopyToScreen`, `GRPort_Init`, `GRPort_FillColor`,
 * `ChangeColor`, `load_opening_screen_art` -- are what that name predicts. It is a
 * suggestion the call graph agrees with: a byte match proves the code, never the name.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
extern void *wing_dib_pointer(GrafRec *graf);
extern int MSGrafOrient(GrafRec *graf);

void MSLockGraf(GRPort *port)
{
    register int y;

    if (port->bits == 0) {
        port->bits = (unsigned char *)wing_dib_pointer((GrafRec *)port->win);
        if (port->bits != 0) {
            port->rows = (long *)mem_new(port->h * 4);
            if (MSGrafOrient((GrafRec *)port->win) == 0) {
                port->rows[0] = (long)(port->h - 1) * port->stride;
                for (y = 1; port->h > y; y++)
                    port->rows[y] = port->rows[y - 1] - port->stride;
            } else {
                port->rows[0] = 0;
                for (y = 1; port->h > y; y++)
                    port->rows[y] = port->rows[y - 1] + port->stride;
            }
        }
    }
}
