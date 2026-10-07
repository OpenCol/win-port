/* listbox_get_text_1050_8d5d: candidate for 1050:8d5d (38 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `listbox_get_text_1070_54b4` on the control's window handle at +0x10: the dialog layer's
 * way of asking a list-box control for the text of item `i`, given the control record rather
 * than the window.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void listbox_get_text_1050_8d5d(DlgControl *control, int i, char *buf)
{
    listbox_get_text_1070_54b4(control->hwnd, i, buf);
}
