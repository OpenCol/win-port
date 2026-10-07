/* set_all_dib_colours: candidate for 1070:a9c0 (38 bytes).  Verdict: RELOCATABLE.
 * symbol: set_all_dib_colours
 *
 * wing_set_dib_colours over the whole 256-entry table: the WinG surface (a GrafRec, whose hdc gets WinGSetDIBColorTable) and the palette passed
 * through, `start` 0 and `count` 0x100 appended.  The four words are two pointers, not
 * four ints -- `wing_set_dib_colours`'s own definition is what says so, and
 * `GRPort_ColorSync` pushes a segment and an offset for each.  The callee is
 * -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from text_call_a93c: wing_set_dib_colours over all 256 entries, with
 * start 0 and count 0x100; GRPort_ColorSync calls it with the port's DC and the colour table.
 */

#include "colonization.h"

#include "records.h"
extern int g_stub_a;

extern void wing_set_dib_colours(void *graf, void *pal, int start, int count);

void set_all_dib_colours(void *w, void *pal)
{
    wing_set_dib_colours((GrafRec *)w, pal, 0, 0x100);
}
