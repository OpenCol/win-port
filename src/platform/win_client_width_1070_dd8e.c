/* win_client_width_1070_dd8e: candidate for 1070:dd8e (37 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: win_client_width_1070_dd8e
 *
 * The client width of the window the object holds at +6.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
extern int win_client_width_1078_23db(void *win);

int win_client_width_1070_dd8e(DialogWin *o)
{
    return win_client_width_1078_23db((MSWin *)o->win);
}
