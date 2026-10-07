/* ColChild_InitCWin_style_nc: candidate for 1048:01c2 (61 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_InitCWin_style_nc
 * name-source: sibling-build
 *
 * The full form the three `ColChild_InitCWin` forwarders at 1048:00d9, 1048:0111 and 1048:0189 sit on (1048:014a, ColChild_InitCWin_nc_parent, calls ColChild_InitCWin_style_ncparent directly): `ColChild_InitCWin_style_ncparent`
 * with every argument passed through and the parent window supplied as NULL. The forwarders'
 * `a`, `b` and `c` are ColChild_InitCWin_style_ncparent's `style`, `save` and `noclose`.
 *
 * `save` is `char`, as on the Mac and in ColChild_InitCWin_style_ncparent's definition: a char
 * passed on to a char parameter is still pushed as the whole word. It was `int` because the
 * callee was declared with `int` here (substitution, 2026-09-28). `noclose` is a Mac `char` too,
 * and is `char` here since the same day: the definition and its five callers' declarations
 * grade clean with it.
 *
 * The call has a NOP before its `push cs`, the five-byte form TLINK leaves for an `extern`,
 * so the callee is declared here and not defined (rule 47).
 *
 * RENAMED 2026-09-26 from win_init_full: the Mac build's ColChild::InitCWin(char*, short x6, char, char), the overload with both the style word and the no-close flag. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void ColChild_InitCWin_style_ncparent(ColChild *o, char *title, int which, int x, int y, int w, int h, int style, char save, char noclose, void *parent);

void ColChild_InitCWin_style_nc(ColChild *o, char *title, int which, int x, int y,
                                 int w, int h, int style, char save, char noclose)
{
    ColChild_InitCWin_style_ncparent(o, title, which, x, y, w, h, style, save, noclose, 0);
}
