/* mouse_right_down_1060_a830: candidate for 1060:a830 (23 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `mouse_right_down_1078_2cfb`, named for the routine it reaches. One of
 * FORTY such forwarders left in the image, one or two per module, each reaching a
 * routine of the widget layer through a call; fifteen of them reach MSDoAllTasks.
 *
 * 23 bytes, not the census 19: the trailing `return` is written out, which costs a
 * second, unreachable epilogue (rule 44).
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

int mouse_right_down_1060_a830(void)
{
    return mouse_right_down_1078_2cfb();
}
