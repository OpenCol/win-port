/* port_select_if_changed -- shipped at 1068:5761.
 *
 * Ported by tools/port.py from matched/gfx/port_select_if_changed.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 get_w_406(void *obj);
extern void text_call_a93c(void *hdc, void *obj);

void port_select_if_changed(Port *p, void *obj)
{
    if (get_w_406(obj) != (*((col_i16 *)((unsigned char *)(p) + 0x20)))) {
        text_call_a93c((*((void * *)((unsigned char *)(p) + 0x22))), obj);
        (*((col_i16 *)((unsigned char *)(p) + 0x20))) = get_w_406(obj);
    }
}
