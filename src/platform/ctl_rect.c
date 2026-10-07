/* ctl_rect: candidate for 1070:9704 (27 bytes).  Verdict: EXACT.
 *
 * The record's member 8 bytes in, as a pointer.
 *
 * RENAMED 2026-09-26 from ptr_plus_8: the address of the control's RECT at +8 (DlgControl's rect); every caller
 * passes a control and reads it as *(RECT *).
 */

#include "colonization.h"

#include "w16types.h"
RECT *ctl_rect(char *ctl)
{
    return (RECT *)(ctl + 8);
}
