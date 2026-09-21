/* port_bind_anim -- shipped at 1068:af2a.
 *
 * Ported by tools/port.py from matched/gfx/port_bind_anim.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void port_bind_anim(Port *p, void *anim)
{
    (*((char *)((unsigned char *)(p) + 0xfa))) = 1;
    (*((void * *)((unsigned char *)(p) + 0xfc))) = anim;
}
