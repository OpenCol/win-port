/* ColChild_AddBucksToMessage: candidate for 1048:11c4 (59 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_AddBucksToMessage
 *
 * Formats a money amount and appends it to the window's message.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_AddStringToMessage(void *message, char *s);
void ColChild_AddBucksToMessage(ColChild *w, long bucks)
{
    char buf[0x14];

    buf[0] = 0;
    say_bucks(buf, bucks);
    ColChild_AddStringToMessage(w, buf);
}
