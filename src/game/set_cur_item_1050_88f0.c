/* set_cur_item_1050_88f0: candidate for 1050:88f0 (24 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a word at +0x2a of a record. One of TWO copies.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_cur_item_1050_88f0(SRadioSet *radios, int cur_item)
{
    radios->cur_item = cur_item;
}
