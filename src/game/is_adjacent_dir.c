/* is_adjacent_dir -- shipped at 1040:7ca4.
 *
 * Ported by tools/port.py from matched/game/is_adjacent_dir.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

int is_adjacent_dir(int a, int b)
{
 int r = 0;

 if (((b + 1) & 7) == a || ((b - 1) & 7) == a)
 r = 1;
 return r;
}
