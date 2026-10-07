/* label_remove_label: candidate for 1010:3684 (122 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: label_remove_label
 * name-source: sibling-build
 *
 * Module 1010's copy of the same idea as `europe_remove_label` (1048:3c6c): delete the
 * module's one global window, at DGROUP:0x9ec, and put its three ids at 0x9f0, 0x9f2 and
 * 0x9f4 back to -1. Five callers, `ColonyWin_ColonyMouseMove` among them, which is what
 * makes it the colony screen's.
 *
 * The two functions differ only in how many ids they reset: twelve bytes of stores, and
 * nothing else. Everything below the stores -- both null tests, the unwinder counter moved
 * by six, the destructor with flag 3, the exception frame -- Borland writes from one
 * `delete`.
 *
 * The six is the headcount of rule 49: `1 + the number of destructible sub-objects`, and
 * GameWin has five, so the same class is being deleted here.
 *
 * `GameWin::~GameWin()` is DEFINED here and not merely declared: rule 47 covers the calls the
 * compiler writes, so the `delete` reaches the destructor through the four-byte `PUSH CS /
 * CALL NEAR` the shipped has, and no jump displacement has to be forgiven.
 *
 * RENAMED 2026-09-26 from close_colony_popup: the Mac build's label_remove_label(void), bound by call-sequence alignment (1.0 aligned gaps in ColonyWin_Show leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.98x this one.
 */

#include "colonization.h"

#pragma option -a
#include "w16types.h"
#include "classes.h"
#pragma option -a-
class DlgControl;
/* Defined, not merely declared: the shipped `delete` calls the destructor through the
 * four-byte `PUSH CS / CALL NEAR`, which the compiler emits only for a callee in this
 * translation unit (rule 47). */

extern GameWin *g_colony_popup;     /* DGROUP:0x9ec */

extern "C" void label_remove_label(void)
{
    g_labelled_building = -1;
    g_labelled_goods = -1;
    g_labelled_cell = -1;
    if (g_colony_popup != 0)
        delete g_colony_popup;
    g_colony_popup = 0;
}
