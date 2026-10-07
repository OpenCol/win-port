/* MrTimer_dtor: candidate for 1068:9243 (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MrTimer::~MrTimer()
 *
 * The destructor of MrTimer: destroys its hidden timer window (the handle at +0) through destroy_window.
 */

#include "colonization.h"

#include "classes.h"

MrTimer::~MrTimer()
{
    destroy_window(hwnd);
}
