/* MSCloseFile_1050_90e6 -- shipped at 1050:90e6.
 *
 * Ported by tools/port.py from matched/game/MSCloseFile_1050_90e6.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSCloseFile(void *p);

void * MSCloseFile_1050_90e6(void *p)
{
    return MSCloseFile(p);
}
