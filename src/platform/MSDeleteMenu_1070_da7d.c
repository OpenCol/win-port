/* MSDeleteMenu_1070_da7d: candidate for 1070:da7d (34 bytes).  Verdict: RELOCATABLE.
 *
 * `DeleteMenu(hmenu, id, MF_BYCOMMAND)` on a non-null menu handle.
 *
 * THIS IS A PER-MODULE COPY of `MSDeleteMenu` at `1070:dba5`, 0x128 bytes later in the
 * same segment: the binary duplicates this layer's helpers once per module that uses them,
 * and both copies compile from the same source. The suffix marks which copy, not a doubt
 * about the name.
 *
 * The id is passed through UNCHANGED, and that is the interesting byte. Every other
 * wrapper in this family subtracts one -- `MSGetSubMenu`, `MSCheckMenuItem` and
 * `MSDeleteMenuItem` all do -- because the layer numbers menu POSITIONS from 1 where Win16
 * numbers them from 0. This one passes `MF_BYCOMMAND` (0) rather than `MF_BYPOSITION`, so
 * its argument is a command id and not a position, and a command id has no offset to undo.
 * The convention is about positions, not about arguments, and this function is the case
 * that shows the difference.
 */

#include "colonization.h"

void MSDeleteMenu_1070_da7d(unsigned hmenu, unsigned id)
{
    if (hmenu != 0)
        DeleteMenu(hmenu, id, 0);
}
