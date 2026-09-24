/* popup_edit_item -- shipped at 1050:142a.
 *
 * Ported by tools/port.py from matched/game/popup_edit_item.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy, STRLEN -> strlen, STRNCPY -> strncpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own Item dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: maxlen -> id
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 font_text_extent_1050_8391(void *font, char *s);

/* 0x14 bytes */
typedef struct {
    char pad[0x6e]; void *font; char pad2[0x7a - 0x72]; col_i16 count; char pad3[0x352 - 0x7c];
    col_i16 label_w; char pad4[0x35e - 0x354]; col_i16 width; char pad5[0x386 - 0x360]; Item *edits;
    char pad6[0x392 - 0x38a]; char block[1];
} List;
extern void * heap_get(void *block, col_i16 n);




extern col_i16 * max_ptr_1050_97d1(col_i16 *a, col_i16 *b);
extern col_i16 * min_ptr_1050_979f(col_i16 *a, col_i16 *b);

/* same object: 4-byte `push cs; call ` (rule 47) */


Item * popup_edit_item(List *p, char *label, char *text, col_i16 maxlen)
{
    Item *tail;
    Item *it;
    col_i16 w;
    col_i16 lw;
    col_i16 cap;

    tail = 0;
    it = p->edits;
    while (it != 0) {
        tail = it;
        it = (*((void * *)((unsigned char *)(it) + 0x10)));
    }
    it = (Item *)heap_get(p->block, 0x14);
    if (tail != 0)
        (*((void * *)((unsigned char *)(tail) + 0x10))) = it;
    else
        p->edits = it;
    (*((void * *)((unsigned char *)(it) + 0x10))) = 0;
    (*((col_i16 *)((unsigned char *)(it) + 0x0))) = 0;
    (*((char * *)((unsigned char *)(it) + 0x8))) = (char *)heap_get(p->block, strlen(label) + 2);
    strcpy((*((char * *)((unsigned char *)(it) + 0x8))), label);
    strcat((*((char * *)((unsigned char *)(it) + 0x8))), " ");
    lw = font_text_extent_1050_8391(p->font, (*((char * *)((unsigned char *)(it) + 0x8)))) + font_text_extent_1050_8391(p->font, " ");
    p->label_w = *max_ptr_1050_97d1(&p->label_w, &lw);
    maxlen = *min_ptr_1050_979f((cap = 0xff, &cap), &maxlen);
    it->id = maxlen;
    (*((char * *)((unsigned char *)(it) + 0xc))) = (char *)heap_get(p->block, 0x100);
    (*((col_i16 *)((unsigned char *)(it) + 0x2))) = font_text_extent_1050_8391(p->font, "W") * maxlen;
    w = p->label_w + (*((col_i16 *)((unsigned char *)(it) + 0x2)));
    (*((col_i16 *)((unsigned char *)(p) + 0x2))) = *max_ptr_1050_97d1(&(*((col_i16 *)((unsigned char *)(p) + 0x2))), &w);
    if (text == 0) {
        ((char * *)((unsigned char *)(it) + 0xc))[0] = 0;
    } else {
        strncpy((*((char * *)((unsigned char *)(it) + 0xc))), text, maxlen);
        ((char * *)((unsigned char *)(it) + 0xc))[maxlen] = 0;
    }
    (*((col_i16 *)((unsigned char *)(it) + 0x6))) = p->count++;
    return it;
}
