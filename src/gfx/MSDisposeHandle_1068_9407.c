/* MSDisposeHandle_1068_9407 -- shipped at 1068:9407.
 *
 * Ported by tools/port.py from matched/gfx/MSDisposeHandle_1068_9407.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSDisposeHandle(col_u16 h);

void * MSDisposeHandle_1068_9407(col_i16 a)
{
    return MSDisposeHandle(a);
}
