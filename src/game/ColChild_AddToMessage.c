/* ColChild_AddToMessage: candidate for 1048:112b (40 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_AddToMessage
 *
 * Appends text string idx (strings) to the message.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern char *strings(int n);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_AddStringToMessage(ColChild *m, char *s);
extern "C"   /* PORT: C linkage: C files call it too (W2) */
void ColChild_AddToMessage(ColChild *child, int idx)
{
    ColChild_AddStringToMessage(child, strings(idx));
}
