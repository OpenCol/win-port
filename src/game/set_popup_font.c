/* set_popup_font: candidate for 1050:00b6 (22 bytes).  Verdict: RELOCATABLE.
 *
 * Stores a pointer at DGROUP:0x24f0, the variable
 * `reset_popup_font` points at the module's own buffer.
 *
 * RENAMED 2026-09-26 from set_ptr_24f0: stores the font pointer Popup_ctor, Popup_clear and Popup_init
 * hand to Popup_set_font (g_popup_font).
 */

#include "colonization.h"

#include "classes.h"

void set_popup_font(Font *p)
{
    g_popup_font = p;
}
