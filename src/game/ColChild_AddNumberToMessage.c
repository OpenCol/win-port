/* ColChild_AddNumberToMessage: candidate for 1048:1153 (56 bytes, 23 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_AddNumberToMessage
 *
 * Appends an int in base 10 to the window's message.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ColChild_AddStringToMessage(void *message, char *s);
void ColChild_AddNumberToMessage(ColChild *w, int number)
{
    char buf[0x14];

    LTOA1(number, buf, 10);
    ColChild_AddStringToMessage(w, buf);
}
