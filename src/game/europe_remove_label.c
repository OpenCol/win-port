/* europe_remove_label: candidate for 1048:3c6c (110 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: europe_remove_label
 * name-source: sibling-build
 *
 * Deletes the module's one global window, at DGROUP:0x1fd0, and puts its id at 0x1fd4 back
 * to -1. Three callers: `europe_label_cargo`, which builds the window and closes any previous one
 * first, and `EuropeWin_Show` and `EuropeWin_MouseMove`, which is what makes it the
 * Europe screen's popup rather than some other module's.
 *
 * THE WHOLE FUNCTION IS ONE `delete`. Borland writes the rest: a copy of the pointer to a
 * local, a second null test the source does not contain, the unwinder counter moved by six,
 * the destructor with flag 3, and an exception frame around all of it in case the destructor
 * throws. The outer null test IS in the source -- `delete` guards itself, so a bare
 * `delete p` would emit only the inner one -- and the clear at the end is unconditional,
 * which is why both null tests jump to it rather than past it.
 *
 * THE SIX IS A HEADCOUNT (rule 49). `ADD DWORD SS:[0x10],n` at a delete site is `1 + the
 * number of destructible sub-objects of the class`, measured by building the same delete
 * against classes with one, two, three and five of them and reading 2, 3, 4 and 6 back.
 * GameWin has exactly five: `GRPort` and `DialogWin` are its bases, `DialogWin` brings
 * `EventWin`, and `EventWin` brings `Screen` and `EventTable`. That is the hierarchy
 * `GameWin_ctor_1048_dc74` already records, and this immediate is an independent witness to
 * it -- get the shape wrong and the byte is wrong.
 *
 * `GameWin::~GameWin()` is DEFINED here and not merely declared: rule 47 covers the calls the
 * compiler writes, so the `delete` reaches the destructor through the four-byte `PUSH CS /
 * CALL NEAR` the shipped has, and no jump displacement has to be forgiven.
 *
 * RENAMED 2026-09-26 from close_europe_popup: the Mac build's europe_remove_label(void), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller MouseMove; caller Show; caller europe_label_cargo), the runner-up gets 2, and the arity agrees (0). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
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

extern GameWin *g_europe_popup;     /* DGROUP:0x1fd0 */

extern "C" void europe_remove_label(void)
{
    g_europe_popup_id = -1;
    if (g_europe_popup != 0)
        delete g_europe_popup;
    g_europe_popup = 0;
}
