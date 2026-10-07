/* gui_teardown: candidate for 1070:b121 (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Three calls and nothing else: `spin_over_count`,
 * `free_wing_library`, `destroy_MSMMWindow_window`.
 *
 * `gui_shutdown` describes it from the outside -- "tears the widget layer down" -- and
 * this is the whole of it.
 */

#include "colonization.h"

extern void free_wing_library(void);

void gui_teardown(void)
{
    spin_over_count();
    free_wing_library();
    destroy_MSMMWindow_window();
}
