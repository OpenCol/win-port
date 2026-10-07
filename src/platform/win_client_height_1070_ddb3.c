/* win_client_height_1070_ddb3: candidate for 1070:ddb3 (37 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: win_client_height_1070_ddb3
 *
 * The client height of the window the object holds at +6.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
extern int win_client_height_1078_241a(void *win);

int win_client_height_1070_ddb3(DialogWin *o)
{
    return win_client_height_1078_241a((MSWin *)o->win);
}
