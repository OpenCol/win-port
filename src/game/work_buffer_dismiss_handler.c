/* work_buffer_dismiss_handler: candidate for 1048:ae31 (27 bytes).  Verdict: RELOCATABLE.
 *
 * A click handler of the memory dialog (show_work_buffer_dialog): sets the flag at DGROUP:0x1fdc and reports 1.
 */

#include "colonization.h"

int work_buffer_dismiss_handler(void)
{
    g_work_buffer_dismissed = 1;
    return 1;
}
