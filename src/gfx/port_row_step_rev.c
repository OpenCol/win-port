/* port_row_step_rev -- shipped at 1068:6255.
 *
 * Ported by tools/port.py from matched/gfx/port_row_step_rev.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 is_state_one(void *hdc);

unsigned char * port_row_step_rev(Port *p, unsigned char *q)
{
    if (is_state_one((*((void * *)((unsigned char *)(p) + 0x22)))) == 0)
        return q + (*((col_i16 *)((unsigned char *)(p) + 0x6)));
    else
        return q - (*((col_i16 *)((unsigned char *)(p) + 0x6)));
}
