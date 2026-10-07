/* port_row_offsets: candidate for 1068:dce4 (29 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * The pointer at +0x1c of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_pixels: the port's row-offset table. MSLockGraf allocates it beside the
 * surface and fills one long per scanline, stepping by the stride bottom-up or top-down with the DIB;
 * wipe_rows hands it to asm_wipe_rows with the surface. The earlier name read it as the pixels.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void *port_row_offsets(GRPort *port)
{
    return port->rows;
}
