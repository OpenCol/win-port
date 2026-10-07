/* MSGetSubMenu: candidate for 1070:d9e3 (41 bytes).  Verdict: RELOCATABLE.
 *
 * GetSubMenu(hmenu, pos - 1): the layer numbers menu positions from 1. 0 for a null menu.
 */

#include "colonization.h"

unsigned MSGetSubMenu(unsigned hmenu, int pos)
{
    if (hmenu != 0)
        return GetSubMenu(hmenu, pos - 1);
    else
        return 0;
}
