/* ColChild_InitCWin_nc: candidate for 1048:0111 (57 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The nine-argument entry that passes the full form's `save` and `noclose` through and
 * supplies ZERO for `style`. One of four forwarders back to back at 1048:00d9..01c2; they
 * differ only in which of ColChild_InitCWin_style_ncparent's `style`, `save` and `noclose` they zero, and
 * the name says which two survive. (It was `ColChild_InitCWin_nc`, from when those three were a, b, c.)
 *
 * The callee sits in this segment but every call here has a NOP before the `push cs`,
 * the five-byte form TLINK leaves when it shortens a call to an `extern`, so it is
 * declared and not defined (rule 47).
 *
 * RENAMED 2026-09-26 from win_init_save_noclose: the Mac build's ColChild::InitCWin(char*, short x5, char, char), the overload with the no-close flag after the save flag. The suffix tells the overloads apart, as GRPort_Init_wh does. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void ColChild_InitCWin_style_nc(ColChild *o, char *title, int which, int x, int y,
                                         int w, int h, int style, char save, char noclose);

void ColChild_InitCWin_nc(ColChild *o, char *title, int which, int x, int y,
                     int w, int h, int save, int noclose)
{
    ColChild_InitCWin_style_nc(o, title, which, x, y, w, h, 0, save, noclose);
}
