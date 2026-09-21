/* anim_unbind -- shipped at 1068:3401.
 *
 * Ported by tools/port.py from matched/gfx/anim_unbind.c.
 *
 * The file's own Anim dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void port_unbind_anim(void *p);

void anim_unbind(Anim *a)
{
    port_unbind_anim((*((void * *)((unsigned char *)(a) + 0x570))));
}
