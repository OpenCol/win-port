/* ColChild_InitCWin_style: candidate for 1048:0189 (57 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The nine-argument entry that passes the full form's `style` and `save` through and
 * supplies ZERO for `noclose`. The mirror of `ColChild_InitCWin_nc` two functions earlier.
 * (It was `ColChild_InitCWin_style`, from when those three were a, b, c.)
 *
 * The callee sits in this segment but every call here has a NOP before the `push cs`,
 * the five-byte form TLINK leaves when it shortens a call to an `extern`, so it is
 * declared and not defined (rule 47).
 *
 * RENAMED 2026-09-26 from win_init_style_save: the Mac build's ColChild::InitCWin(char*, short x6, char), the overload with a style word before the save flag. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void ColChild_InitCWin_style_nc(ColChild *o, char *title, int which, int x, int y,
                                         int w, int h, int style, char save, char noclose);

void ColChild_InitCWin_style(ColChild *o, char *title, int which, int x, int y,
                     int w, int h, int style, int save)
{
    ColChild_InitCWin_style_nc(o, title, which, x, y, w, h, style, save, 0);
}
