/* text_line_append -- shipped at 1050:0966.
 *
 * Ported by tools/port.py from matched/game/text_line_append.c.
 *
 * STRCPY -> strcpy, STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own Node dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

typedef struct {
    char pad0[0x78];
    col_i16 count; /* +0x78 */
    char pad7a[0x86 - 0x7a];
    col_i16 flags; /* +0x86 */
    char pad88[0x376 - 0x88];
    Node *head; /* +0x376 */
    char pad37a[0x392 - 0x37a];
    char heap[2]; /* +0x392 */
} TextList;
extern void * heap_get(void *heap, col_i16 size);



Node * text_line_append(TextList *o, char *s)
{
    Node *prev = 0;
    Node *n;

    n = o->head;
    while (n != 0) {
        prev = n;
        n = (*((void * *)((unsigned char *)(n) + 0x6)));
    }
    n = (Node *)heap_get(o->heap, 10);
    if (prev != 0)
        (*((void * *)((unsigned char *)(prev) + 0x6))) = n;
    else
        o->head = n;
    (*((void * *)((unsigned char *)(n) + 0x6))) = 0;
    (*((col_i16 *)((unsigned char *)(n) + 0x0))) = 0;
    if ((col_i16)*s == 0x5e) {
        s++;
        if ((col_i16)*s == 0x5e) {
            s++;
            o->flags |= 1;
        } else {
            o->flags |= 2;
        }
    }
    (*((char * *)((unsigned char *)(n) + 0x2))) = (char *)heap_get(o->heap, strlen(s) + 1);
    strcpy((*((char * *)((unsigned char *)(n) + 0x2))), s);
    o->count++;
    return n;
}
