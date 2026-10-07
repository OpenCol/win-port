/* MapWin_ShowMap_all: candidate for 1038:a6cf (27 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * This module's one-line forwarder to `copy_all_to_screen_1038_e428`, named for the routine it reaches.
 *
 * The callee is reached by the four-byte `PUSH CS / CALL NEAR` form, so it is in this
 * translation unit and appears here as a stub (rule 47).
 *
 * RENAMED 2026-09-26 from copy_all_to_screen_1038_a6cf: the Mac build's MapWin::ShowMap(void), the whole-window overload: the free ShowMap() -- bound by alignment and layout -- is exactly a call of this with &g_map_window. MapWin_ShowMap is the four-argument region overload, so this takes _all, as GRPort_FillColor_all does. The other copy_all_to_screen_* copies belong to other window modules.
 */

#include "colonization.h"

#include "classes.h"
/* same object: 4-byte `push cs; call ` */

void MapWin_ShowMap_all(void *p)
{
    copy_all_to_screen_1038_e428((GameWin *)p);
}
