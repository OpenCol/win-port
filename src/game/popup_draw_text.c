/* popup_draw_text -- shipped at 1050:1ae6.
 *
 * Ported by tools/port.py from matched/game/popup_draw_text.c.
 *
 * The file's own FontRec, Popup dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 font_text_extent_1050_8391(FontRec *f, const char *s);

extern unsigned char TextColor(unsigned char v);
extern void draw_string_shadowed(void *port, FontRec *f, const char *s,
                                     col_i16 x, col_i16 y, col_i16 align);

/* same object: 4-byte `push cs; call ` */


void popup_draw_text(Popup *p)
{
    col_i16 i;
    char *s;
    char *q;
    char *end;
    char qsave;
    char endsave;
    col_i16 prev;
    col_i16 markw;
    register col_i16 acc;
    register col_i16 adv;

    i = 0;
    markw = font_text_extent_1050_8391((*((FontRec * *)((unsigned char *)(p) + 0x6e))), "{");
    acc = 0;
    prev = ((col_i16 *)((unsigned char *)(p) + 0x2ac))[i];
    while (((char * *)((unsigned char *)(p) + 0xac))[i] != 0) {
        s = q = ((char * *)((unsigned char *)(p) + 0xac))[i];
        end = s + ((col_i16 *)((unsigned char *)(p) + 0x1ac))[i];
        endsave = *end;
        *end = 0;
        adv = ((col_i16 *)((unsigned char *)(p) + 0x22c))[i];
        if (((col_i16 *)((unsigned char *)(p) + 0x2ac))[i] == prev)
            adv -= acc;
        else
            acc = 0;
        prev = ((col_i16 *)((unsigned char *)(p) + 0x2ac))[i];
        while (*s != 0) {
            switch (*s) {
            case 0x7b:
                TextColor((*((unsigned char *)((unsigned char *)(p) + 0x96))));
                s++;
                q++;
                acc += markw;
                continue;
            case 0x7d:
                TextColor((*((unsigned char *)((unsigned char *)(p) + 0x92))));
                s++;
                q++;
                acc += markw;
                continue;
            case 0x7c:
                goto next;
            default:
                break;
            }
            do {
                q++;
            } while ((col_i16)*q != 0x7b && (col_i16)*q != 0x7d && *q != 0 && (col_i16)*q != 0x7c);
            qsave = *q;
            *q = 0;
            draw_string_shadowed((*((void * *)((unsigned char *)(p) + 0x0))), (*((FontRec * *)((unsigned char *)(p) + 0x6e))), s, (*((col_i16 *)((unsigned char *)(p) + 0x33c))) + adv, (*((col_i16 *)((unsigned char *)(p) + 0x33e))) + ((col_i16 *)((unsigned char *)(p) + 0x2ac))[i], 5);
            adv += font_text_extent_1050_8391((*((FontRec * *)((unsigned char *)(p) + 0x6e))), s);
            *q = qsave;
            s = q;
            continue;
        }
        *end = endsave;
next:
        i++;
    }
}
