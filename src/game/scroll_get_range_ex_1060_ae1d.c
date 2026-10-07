/* scroll_get_range_ex_1060_ae1d: candidate for 1060:ae1d (51 bytes).  Verdict: RELOCATABLE.
 * symbol: scroll_get_range_ex_1060_ae1d
 *
 * Reads the vertical scroll range of the object's window through scroll_get_range_ex_1070_604d.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern char g_stub_s[];
extern void scroll_get_range_ex_1070_604d(void *win, int *lo, int *hi, int flag);

extern void *get_win_1060_ab6e(void *o);

void scroll_get_range_ex_1060_ae1d(void *o, int *lo, int *hi)
{
    scroll_get_range_ex_1070_604d((MSWin *)(get_win_1060_ab6e((Screen *)o)), lo, hi, 1);
}

