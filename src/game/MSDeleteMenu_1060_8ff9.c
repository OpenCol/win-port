/* MSDeleteMenu_1060_8ff9 -- shipped at 1060:8ff9.
 *
 * Ported by tools/port.py from matched/game/MSDeleteMenu_1060_8ff9.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSDeleteMenu(void *p);

void MSDeleteMenu_1060_8ff9(col_i16 a, col_i16 b, void *p)
{
    MSDeleteMenu(p);
}
