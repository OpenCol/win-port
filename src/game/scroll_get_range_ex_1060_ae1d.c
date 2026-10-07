/* PORT: this file's own declarations of get_win_1060_ab6e, scroll_get_range_ex_1070_604d removed: the header declares them from the definition (W1 step 3). */
/* scroll_get_range_ex_1060_ae1d: candidate for 1060:ae1d (51 bytes).  Verdict: RELOCATABLE.
 * symbol: scroll_get_range_ex_1060_ae1d
 *
 * Reads the vertical scroll range of the object's window through scroll_get_range_ex_1070_604d.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern char g_stub_s[];


void scroll_get_range_ex_1060_ae1d(void *o, int *lo, int *hi)
{
    scroll_get_range_ex_1070_604d((MSWin *)(get_win_1060_ab6e((Screen *)o)), lo, hi, 1);
}

