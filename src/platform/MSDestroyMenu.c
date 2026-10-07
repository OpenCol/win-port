/* MSDestroyMenu: candidate for 1070:d613 (29 bytes).  Verdict: RELOCATABLE.
 * symbol: MSDestroyMenu
 *
 * DestroyMenu on a non-null handle. The handle is enregistered into SI, which is what the
 * shipped `MOV SI,[BP+6] / OR SI,SI` says; it is used twice, once to test and once to pass.
 */

#include "colonization.h"

void MSDestroyMenu(unsigned hmenu)
{
    if (hmenu != 0)
        DestroyMenu(hmenu);
}
