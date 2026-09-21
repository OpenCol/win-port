/* port_bits -- shipped at 1068:d77f.
 *
 * Ported by tools/port.py from matched/gfx/port_bits.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void * port_bits(Port *p)
{
    return (*((void * *)((unsigned char *)(p) + 0x2)));
}
