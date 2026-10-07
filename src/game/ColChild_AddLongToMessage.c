/* ColChild_AddLongToMessage: candidate for 1048:118b (57 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends a long in base 10 to the window's message -- the 32-bit sibling of
 * `ColChild_AddNumberToMessage` at 1048:1153, immediately after it in the same segment (1048:1153 + 56 bytes = 1048:118b). The only
 * differences are `ltoa` in place of `LTOA1` and a 4-byte argument.
 */

#include "colonization.h"

#include "w16types.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
#include "classes.h"
extern void ColChild_AddStringToMessage(ColChild *w, char *s);
void ColChild_AddLongToMessage(ColChild *w, long value)
{
    char buf[0x14];

    ltoa_rtl(value, buf, 10);
    ColChild_AddStringToMessage(w, buf);
}
