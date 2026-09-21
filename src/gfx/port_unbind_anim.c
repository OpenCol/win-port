/* port_unbind_anim -- shipped at 1068:af4e.
 *
 * Ported by tools/port.py from matched/gfx/port_unbind_anim.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void port_unbind_anim(Port *p)
{
    (*((char *)((unsigned char *)(p) + 0xfa))) = 0;
    (*((void * *)((unsigned char *)(p) + 0xfc))) = 0;
}
