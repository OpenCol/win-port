/* mode_dispatch: candidate for 1010:e145 (59 bytes).  Verdict: RELOCATABLE.
 * symbol: mode_dispatch
 *
 * Calls one of three handlers for the mode byte at DGROUP:0xc0f. The case bodies are laid
 * out in the reverse of the order the comparisons test them, which is what says the source
 * wrote them 2, 1, 0 (rule 36).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void mode_dispatch(void)
{
    switch (g_colony_mode) {
    case 2:
        empty_1010_e129();
        break;
    case 1:
        empty_1010_e0f1();
        break;
    case 0:
        empty_1010_e137();
        break;
    }
}
