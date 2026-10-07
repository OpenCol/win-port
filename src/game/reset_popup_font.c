/* reset_popup_font: candidate for 1050:00cc (24 bytes).  Verdict: RELOCATABLE.
 *
 * Points the pointer at DGROUP:0x24f0 at the buffer at DGROUP:0x24e8 --
 * the segment half is DS, so the offset is an immediate.
 *
 * RENAMED 2026-09-26 from set_ptr_24f0_buf: points g_popup_font back at g_font16, the default.
 */

#include "colonization.h"

#include "classes.h"

void reset_popup_font(void)
{
    g_popup_font = &g_font16;
}
