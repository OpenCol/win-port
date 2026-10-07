/* Screen_LButtonDownNow: candidate for 1060:a819 (23 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * This module's one-line forwarder to `MSLButtonDown`, named for the routine it reaches. One of
 * FORTY such forwarders left in the image, one or two per module, each reaching a
 * routine of the widget layer through a call; fifteen of them reach MSDoAllTasks.
 *
 * 23 bytes, not the census 19: the trailing `return` is written out, which costs a
 * second, unreachable epilogue (rule 44).
 *
 * The callee is a call into another segment and is `extern`.
 *
 * RENAMED 2026-09-26 from mouse_left_down_1060_a819: the Mac build's Screen::LButtonDownNow(void): its one parameter is the unread `this`, and it asks the platform layer (MSLButtonDown) whether the left button is down right now. The width matches.
 */

#include "colonization.h"

#include "classes.h"
extern int MSLButtonDown(void);

int Screen_LButtonDownNow(Screen *w)
{
    return MSLButtonDown();
}
