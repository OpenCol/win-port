/* ColChild_NextMessage -- shipped at 1048:13c7.
 *
 * Ported by tools/port.py from matched/game/ColChild_NextMessage.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: timer -> f776
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void sub_1048_d48b(ColChild *w, char *s);

extern col_i16 start_timer(void ( *fn)(void), col_i16 ms, col_i16 mode);
extern void message_timer_expired(void);



void ColChild_NextMessage(ColChild *w)
{
    register col_i16 i;
    char buf[0x50];

    (*((col_i16 *)((unsigned char *)(w) + 0x778)))--;
    if ((*((col_i16 *)((unsigned char *)(w) + 0x778))) != 0) {
        for (i = 0; i < (*((col_i16 *)((unsigned char *)(w) + 0x778))); i++)
            strcpy(((char *)((unsigned char *)(w) + 0x1e9))[i], ((char *)((unsigned char *)(w) + 0x1e9))[i + 1]);
        strcpy(buf, ((char *)((unsigned char *)(w) + 0x1e9))[0]);
        sub_1048_d48b(w, buf);
        w->f776 = start_timer(message_timer_expired, 0x7d0, 1);
    }
}
