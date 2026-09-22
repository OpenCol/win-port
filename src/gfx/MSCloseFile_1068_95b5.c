/* MSCloseFile_1068_95b5 -- shipped at 1068:95b5.
 *
 * Ported by tools/port.py from matched/gfx/MSCloseFile_1068_95b5.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSCloseFile(void *p);

void * MSCloseFile_1068_95b5(void *p)
{
    return MSCloseFile(p);
}
