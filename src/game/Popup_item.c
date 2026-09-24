/* Popup_item -- shipped at 1050:122c.
 *
 * Ported by tools/port.py from matched/game/Popup_item.c.
 *
 * STRCPY -> strcpy, STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own Item, Popup dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: index -> id
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 default_width(Popup *p);
col_i16 font_text_extent_1050_8391(void *font, char *s);

/* 0x12 bytes */
extern void * heap_get(void *block, col_i16 n);



/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_i16 * max_ptr_1050_97d1(col_i16 *a, col_i16 *b);

Item * Popup_item(Popup *p, char *s, col_i16 id)
{
    Item *tail = 0;
    Item *it;
    col_i16 w;

    it = (*((Item * *)((unsigned char *)(p) + 0x36e)));
    while (it != 0) {
        tail = it;
        it = (*((void * *)((unsigned char *)(it) + 0xa)));
    }
    it = (Item *)heap_get(((char *)((unsigned char *)(p) + 0x392)), 0x12);
    if (tail != 0) {
        (*((void * *)((unsigned char *)(tail) + 0xa))) = it;
        (*((void * *)((unsigned char *)(it) + 0xe))) = tail;
    } else {
        (*((Item * *)((unsigned char *)(p) + 0x36e))) = it;
        (*((Item * *)((unsigned char *)(p) + 0x366))) = it;
        (*((void * *)((unsigned char *)(it) + 0xe))) = 0;
    }
    (*((Item * *)((unsigned char *)(p) + 0x372))) = it;
    (*((void * *)((unsigned char *)(it) + 0xa))) = 0;
    (*((col_i16 *)((unsigned char *)(it) + 0x0))) = 0;
    (*((char * *)((unsigned char *)(it) + 0x6))) = (char *)heap_get(((char *)((unsigned char *)(p) + 0x392)), strlen(s) + 2);
    strcpy((*((char * *)((unsigned char *)(it) + 0x6))), s);
    if (*s == 0)
        (*((col_i16 *)((unsigned char *)(it) + 0x0))) |= 1;
    (*((col_i16 *)((unsigned char *)(it) + 0x2))) = id;
    w = font_text_extent_1050_8391((*((void * *)((unsigned char *)(p) + 0x6e))), (*((char * *)((unsigned char *)(it) + 0x6)))) + default_width(p);
    w += (*((col_i16 *)((unsigned char *)(p) + 0x9a)));
    (*((col_i16 *)((unsigned char *)(p) + 0x356))) = *max_ptr_1050_97d1(&w, &(*((col_i16 *)((unsigned char *)(p) + 0x356))));
    (*((col_i16 *)((unsigned char *)(it) + 0x2))) = (*((col_i16 *)((unsigned char *)(p) + 0x7c)))++;
    return it;
}
