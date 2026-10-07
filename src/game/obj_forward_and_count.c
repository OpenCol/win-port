/* obj_forward_and_count: candidate for 1050:86ac (42 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * Passes the object's handle at +0x10 and a pointer on, and bumps a counter at +0x1a.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void obj_forward_and_count(SListBox *listbox, void *p)
{
    listbox_add_string(listbox->hwnd, (const char *)p);
    listbox->count++;
}
