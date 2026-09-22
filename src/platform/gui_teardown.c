/* gui_teardown -- shipped at 1070:b121.
 *
 * Ported by tools/port.py from matched/platform/gui_teardown.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void spin_over_count(void);
extern void free_resource_library(void);
extern void destroy_MSMMWindow_window(void);

void gui_teardown(void)
{
    spin_over_count();
    free_resource_library();
    destroy_MSMMWindow_window();
}
