/* preload_pictures: candidate for 1008:4d0c (149 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: preload_pictures
 * name-source: descriptive
 *
 * Loads picture resources 0x2bc..0x2d4 -- twenty-five of them -- one at a time into a scratch
 * `GRPort` with a scratch `CTable`, pumping the message queue between each. Nothing is kept:
 * both objects are destroyed on the way out and the loaded picture is overwritten by the next
 * one, so the whole function exists for the side effect of loading, which is the resource
 * cache warming up.
 *
 * The pump between iterations is what makes it a start-up step rather than a loop: twenty-five
 * resource loads with no repaint would look like a hang.
 *
 * Its size is the census 149 and not the 178-byte gap: the 29 bytes between are the next
 * function's exception descriptor, which is data.
 */

#include "colonization.h"

#include "fathers.h"

#include "w16types.h"
#include "classes.h"

struct logpal;
extern int GRPort_LoadPicture(GRPort *port, int res, int a, int b,
                                      struct logpal *t);

/* same object: 4-byte `push cs; call ` */

void preload_pictures(void)
{
    GRPort port;
    CTable tbl;
    register int i;

    for (i = 0; i < FATHER_COUNT; i++) {
        GRPort_LoadPicture(&port, i + 0x2bc, 0x10, 0xcc, (struct logpal *)&tbl);
        call_message_pump();
    }
}

