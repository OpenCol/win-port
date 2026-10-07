/* submenu_append_item: candidate for 1060:8f99 (62 bytes).  Verdict: RELOCATABLE.
 * symbol: submenu_append_item
 *
 * MSAppendMenuItem on the handle submenu_of_1060_a938 derives from the window and
 * `a`, with four arguments.  The helper is -called and defined in-file
 * as a stub.
 *
 * RENAMED 2026-09-26 from menu_call_dae6: MSAppendMenuItem on the submenu submenu_of derives from the window
 * and index, with the position, text and id passed through.
 */

#include "colonization.h"

#include "classes.h"
extern int g_stub_a;

extern int submenu_of_1060_a938(void *menu, int pos);

void submenu_append_item(void *w, int index, int position, char *text, unsigned id)
{
    int h;

    h = submenu_of_1060_a938((MenuClass *)w, index);
    MSAppendMenuItem(h, position, text, id);
}
