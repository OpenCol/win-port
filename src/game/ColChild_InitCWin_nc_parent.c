/* PORT: this file's own declarations of ColChild_InitCWin_style_ncparent removed: the header declares it from the definition (W1 step 3). */
/* ColChild_InitCWin_nc_parent: candidate for 1048:014a (63 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_InitCWin_nc_parent
 * name-source: sibling-build
 *
 * `ColChild_InitCWin_style_ncparent` with `style` supplied as ZERO and everything else passed through --
 * the one member of the block at 1048:00d9..01c2 that targets `ColChild_InitCWin_style_ncparent` rather
 * than `ColChild_InitCWin_style_nc`.
 *
 * `save` and `noclose` are `char` in ColChild_InitCWin_style_ncparent's own definition. They are declared
 * `int` HERE because the shipped pushes each as a whole word straight from the frame
 * (`push word [bp+0x18]`), which is what an `int` pass-through compiles to; a `char` one
 * would load the byte and widen it first.
 *
 * The callee sits in this segment but every call here has a NOP before the `push cs`,
 * the five-byte form TLINK leaves when it shortens a call to an `extern`, so it is
 * declared and not defined (rule 47).
 *
 * RENAMED 2026-09-27 from game_window_init_style0: cleans 26 = InitCWin(char*, short x5, char, char, EventWin*); callers PediaWin_Init, ReportWin_Init = the Mac overload's callers
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void ColChild_InitCWin_nc_parent(ColChild *o, char *title, int which, int x, int y,
                                 int w, int h, int save, int noclose, void *parent)
{
    ColChild_InitCWin_style_ncparent(o, title, which, x, y, w, h, 0, save, noclose, parent);
}
