/* MSCloseFile_1068_dcc4 -- shipped at 1068:dcc4.
 *
 * Ported by tools/port.py from matched/gfx/MSCloseFile_1068_dcc4.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSCloseFile(void *p);

void * MSCloseFile_1068_dcc4(void *p)
{
    return MSCloseFile(p);
}
