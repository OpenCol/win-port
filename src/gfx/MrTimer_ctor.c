/* MrTimer_ctor: candidate for 1068:91c8 (123 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MrTimer::MrTimer()
 * name-source: sibling-build
 *
 * The constructor of MrTimer, 123 bytes (0x7b) above `MrTimer_dtor`, immediately before it: clears all sixteen timer slots
 * and their arguments, then creates the daemon window whose HWND lives at +0.
 *
 * The object is 0x6a bytes -- `short hwnd`, seventeen 4-byte slots from +2, seventeen
 * argument words from +0x46, and two bytes of tail. Slot 0 is never touched by anything:
 * `MrTimer_Set` searches 1..16 and so does this.
 *
 * THE ALLOCATION IS THE CONSTRUCTOR'S, not a separate function. Borland passes `this` as
 * the first argument and allocates `sizeof` through `__bnew_qui_rtl` when it arrives null,
 * which is the `CMP DWORD [bp+6],0` at the top; the `INC DWORD [ss:0x10]` at the bottom is
 * the same constructor convention. Writing the class and `MrTimer::MrTimer()` produces all
 * of it, which is why the file has no `new` in it.
 */

#include "colonization.h"

#include "classes.h"

MrTimer::MrTimer()
{
    register int i;

    for (i = 1; i <= 16; i++) {
        slots[i] = 0;
        counts[i] = 0;
    }
    hwnd = create_timer_daemon_window(this);
}
