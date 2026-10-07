/* MenuClass_ctor: candidate for 1060:8f1a (61 bytes, 2 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MenuClass::MenuClass()
 *
 * The constructor of MenuClass: a 2-byte object holding a menu handle, zeroed.
 */

#include "colonization.h"

#include "classes.h"

MenuClass::MenuClass()
{
    hmenu = 0;
}
