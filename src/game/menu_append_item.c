/* menu_append_item: candidate for 1060:8fd7 (34 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSAppendMenu(hmenu, text, id)`, from an entry that takes its arguments in a DIFFERENT
 * order -- menu, id, text -- and one more argument first that it never reads, the shape of
 * a member function that ignores its `this`. It sits between the menu helpers at 1060:8fxx,
 * directly before MSDeleteMenu_1060_8ff9.
 *
 * MSAppendMenu is in the widget segment, a call, so it is extern.
 */

#include "colonization.h"

void menu_append_item(void *unused, unsigned hmenu, unsigned id, char *text)
{
    MSAppendMenu(hmenu, text, id);
}
