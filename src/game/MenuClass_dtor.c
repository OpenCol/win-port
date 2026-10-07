/* MenuClass_dtor: candidate for 1060:8f57 (66 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MenuClass::~MenuClass()
 *
 * The destructor of MenuClass: destroys the menu handle.
 */

#include "colonization.h"

#include "classes.h"

MenuClass::~MenuClass()
{
    MSDestroyMenu(hmenu);
}
