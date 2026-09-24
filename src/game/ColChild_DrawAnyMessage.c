/* ColChild_DrawAnyMessage -- shipped at 1048:12c6.
 *
 * Ported by tools/port.py from matched/game/ColChild_DrawAnyMessage.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: has_own -> active, timer -> f776
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void layout_origin(ColChild *w, col_i16 kind, col_i16 *a, col_i16 *b);
void sub_1048_d48b(ColChild *w, char *s);

extern col_i16 start_timer(void ( *fn)(void), col_i16 ms, col_i16 mode);
extern void message_timer_expired(void);




void ColChild_DrawAnyMessage(ColChild *w, col_i16 mode, char *msg)
{
    col_i16 a;
    col_i16 b;
    char buf[0x50];

    if (w->active != 0 || msg != 0) {
        layout_origin(w, w->kind, &a, &b);
        if (w->active != 0)
            strcpy(buf, ((char *)((unsigned char *)(w) + 0x149)));
        else
            strcpy(buf, msg);
        if ((*((col_i16 *)((unsigned char *)(w) + 0x778))) == 0) {
            if (mode != 0)
                sub_1048_d48b(w, buf);
            w->f776 = start_timer(message_timer_expired, 0x7d0, 1);
            (*((col_i16 *)((unsigned char *)(w) + 0x778)))++;
        } else {
            if ((*((col_i16 *)((unsigned char *)(w) + 0x778))) <= 0xf) {
                strcpy(((char *)((unsigned char *)(w) + 0x1e9))[(*((col_i16 *)((unsigned char *)(w) + 0x778)))], buf);
                (*((col_i16 *)((unsigned char *)(w) + 0x778)))++;
            }
        }
    }
}
