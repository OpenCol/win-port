/* memory_check: candidate for 1048:b9a4 (25 bytes).  Verdict: RELOCATABLE.
 *
 * A Cheat-menu entry that shows one text resource and nothing else. ask_text_resource is
 * -called, so it lives in this object and a stub is enough to place the call.
 *
 * `MEMORY` is the literal at DGROUP:0x241c, and it is attested twice over:
 * analysis/function-anchors.json binds this address to that identifier, and
 * analysis/ghidra/strings.json reads the string out of the image at that exact offset.
 */

#include "colonization.h"

void memory_check(void)
{
    ask_text_resource("MEMORY");
}
