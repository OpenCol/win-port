/* GRPort_ColorSync: candidate for 1068:5761 (81 bytes, 32 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_ColorSync
 * name-source: sibling-build
 *
 * Loads a colour table into the whole 256-entry DIB colour table of the port's WinG surface
 * (the GrafRec pointer at +0x22, via set_all_dib_colours) only when the table's seed differs
 * from the one recorded at +0x20, then records the seed.
 *
 * RENAMED 2026-09-26 from port_select_if_changed: the Mac build's GRPort::ColorSync(CTable&): this plus the colour table, selected into the port's DC only when its id differs from the one recorded. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int ctable_seed(void *obj);
extern void set_all_dib_colours(void *hdc, void *obj);

void GRPort_ColorSync(GRPort *port, CTable *table)
{
    if (ctable_seed(table) != port->current) {
        set_all_dib_colours(port->win, table);
        port->current = ctable_seed(table);
    }
}
