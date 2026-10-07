/* alert_box_1068_d470: candidate for 1068:d470 (44 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `alert_box_1070_66cf`: caption, text, icon and button
 * set passed straight through. Its first 40 bytes are `CTable_CopyRawColors` (which has no dead epilogue) except for the one
 * relocated call word, which the comparator masks -- so the callee's name comes from the
 * disassembly.
 *
 * 44 bytes, not the census 40: the written-out `return` adds a dead second epilogue
 * (rule 44).
 */

#include "colonization.h"

int alert_box_1068_d470(char *caption, char *text, int icon, int buttons)
{
    return alert_box_1070_66cf(caption, text, icon, buttons);
}
