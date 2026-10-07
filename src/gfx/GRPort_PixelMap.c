/* GRPort_PixelMap: candidate for 1068:61a1 (81 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_PixelMap
 *
 * Huge pointer to pixel (x, y) of the port: the base at +0x18 plus the row offset
 * table entry (+0x1c, one long per row) plus x.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

char *GRPort_PixelMap(GRPort *port, int x, int y)
{
    return port->bits + port->rows[y] + x;
}
