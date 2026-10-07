/* ColChild_InitCWin: candidate for 1048:00d9 (56 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_InitCWin
 * name-source: sibling-build
 *
 * The eight-argument entry onto `ColChild_InitCWin_style_nc`: it passes the first seven and
 * its eighth straight through and supplies ZERO for the full form's `a` and `c`. This is the
 * function `ColChild_InitCWin_style_nc`'s record measures itself against -- "the eight of ColChild_InitCWin and
 * two more" -- and the first of four forwarders back to back at 1048:00d9..01c2 that differ
 * only in which of the full form's trailing three they zero.
 *
 * It was `ColChild_InitCWin`; the stem is unique, so the suffix said nothing.
 *
 * The callee sits in this segment but every call here has a NOP before the `push cs`,
 * the five-byte form TLINK leaves when it shortens a call to an `extern`, so it is
 * declared and not defined (rule 47).
 *
 * RENAMED 2026-09-26 from win_init: the Mac build's ColChild::InitCWin(char*, short, short, short, short, short, char): this plus a title, which, x, y, w, h and the save flag -- six words after the title, exactly the Mac overload's. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void ColChild_InitCWin_style_nc(ColChild *o, char *title, int which, int x, int y,
                                         int w, int h, int style, char save, char noclose);

void ColChild_InitCWin(ColChild *o, char *title, int which, int x, int y,
                  int w, int h, char save)
{
    ColChild_InitCWin_style_nc(o, title, which, x, y, w, h, 0, save, 0);
}
