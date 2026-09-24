/* popup_image_item -- shipped at 1050:107b.
 *
 * Ported by tools/port.py from matched/game/popup_image_item.c.
 *
 * STRCPY -> strcpy, STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own Item dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 font_text_extent_1050_8391(void *font, char *s);

/* 0x1c bytes */
typedef struct {
    char pad[0x6e]; void *font; char pad2[0x7e - 0x72]; col_i16 count; char pad3[0xa6 - 0x80];
    col_i16 margin; char pad4[0x35a - 0xa8]; col_i16 width; char pad5[0x36a - 0x35c]; Item *first;
    char pad6[0x37a - 0x36e]; Item *items; char pad7[0x392 - 0x37e]; char block[1];
} List;
extern void * heap_get(void *block, col_i16 n);


extern col_i16 * max_ptr_1050_97d1(col_i16 *a, col_i16 *b);

/* same object: 4-byte `push cs; call ` (rule 47) */


Item * popup_image_item(List *p, void *sprite, col_i16 id, char *s)
{
    Item *tail;
    Item *it;
    col_i16 w;

    tail = 0;
    it = p->items;
    while (it != 0) {
        tail = it;
        it = (*((void * *)((unsigned char *)(it) + 0x14)));
    }
    it = (Item *)heap_get(p->block, 0x1c);
    if (tail != 0) {
        (*((void * *)((unsigned char *)(tail) + 0x14))) = it;
        (*((void * *)((unsigned char *)(it) + 0x18))) = tail;
        (*((void * *)((unsigned char *)(p->items) + 0x18))) = it;
    } else {
        p->items = it;
        p->first = it;
        (*((void * *)((unsigned char *)(p->items) + 0x18))) = 0;
    }
    (*((void * *)((unsigned char *)(it) + 0x14))) = 0;
    (*((col_i16 *)((unsigned char *)(it) + 0x2))) = 0;
    (*((void * *)((unsigned char *)(it) + 0x10))) = sprite;
    w = 0x20;
    if (s != 0) {
        (*((char * *)((unsigned char *)(it) + 0x4))) = (char *)heap_get(p->block, strlen(s) + 1);
        strcpy((*((char * *)((unsigned char *)(it) + 0x4))), s);
        w += font_text_extent_1050_8391(p->font, (*((char * *)((unsigned char *)(it) + 0x4)))) + p->margin;
    } else {
        (*((char * *)((unsigned char *)(it) + 0x4))) = 0;
    }
    (*((col_i16 *)((unsigned char *)(it) + 0x0))) = id;
    p->width = *max_ptr_1050_97d1(&w, &p->width);
    p->count++;
    return it;
}
