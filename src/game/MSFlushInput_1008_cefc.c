/* MSFlushInput_1008_cefc: candidate for 1008:cefc (19 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * This module's one-line forwarder to `MSFlushInput`, named for the routine it reaches. One of
 * FORTY such forwarders left in the image, one or two per module, each reaching a
 * routine of the widget layer through a call; fifteen of them reach MSDoAllTasks.
 *
 * The callee is a call into another segment and is `extern`.
 *
 * RENAMED 2026-09-26 from peek_message_only_1008_cefc: the module forwarder to MSFlushInput (was peek_message_only), named for the routine it reaches: that loops PeekMessage with PM_REMOVE over the mouse and keyboard ranges and discards everything pending -- the opposite of 'peek only'.
 *
 * RENAMED 2026-09-27 from flush_input_messages_1008_cefc: the Mac build's MSFlushInput(void), bound by call-sequence alignment (tools/mac_bind.py): the body drains the mouse (0x200..0x209) and keyboard (0x100..0x108) messages; the forwarders carry its name with their address.
 */

#include "colonization.h"

extern void MSFlushInput(void);

void MSFlushInput_1008_cefc(void)
{
    MSFlushInput();
}
