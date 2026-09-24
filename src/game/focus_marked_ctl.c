/* focus_marked_ctl -- shipped at 1060:9b3f.
 *
 * Ported by tools/port.py from matched/game/focus_marked_ctl.c.
 *
 * The file's own Ctl, Owner dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Ctl * get_p_12_1060_abe3(Ctl *c);

extern char byte_at_16(Ctl *c);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void focus_at_or_after(Owner *o, Ctl *c);
void focus_marked_ctl(Owner *o)
{
    Ctl *c;

    c = (*((Ctl * *)((unsigned char *)(o) + 0x8c)));
    while (c != 0) {
        if (byte_at_16(c)) {
            if ((*((char *)((unsigned char *)(o) + 0x98))) != 0)
                focus_at_or_after(o, c);
            return;
        }
        c = get_p_12_1060_abe3(c);
    }
    c = (*((Ctl * *)((unsigned char *)(o) + 0x8c)));
    if (c != 0) {
        if ((*((char *)((unsigned char *)(o) + 0x98))) != 0)
            focus_at_or_after(o, c);
    }
}
