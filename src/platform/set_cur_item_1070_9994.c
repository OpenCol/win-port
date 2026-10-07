/* set_cur_item_1070_9994: candidate for 1070:9994 (24 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: set_cur_item_1070_9994
 * name-source: descriptive
 *
 * Stores a word at +0x2a of a record. One of TWO copies.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void set_cur_item_1070_9994(SRadioSet *radios, int cur_item)
{
    radios->cur_item = cur_item;
}
