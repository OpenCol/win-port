/* list_unlink_by_id -- shipped at 1060:99dc.
 *
 * Ported by tools/port.py from matched/game/list_unlink_by_id.c.
 *
 * The file's own Item dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

typedef struct { char pad[0x8c]; Item *head; } List;
extern col_i16 get_w_2_1060_abca(void *item);
extern Item * get_p_12_1060_abe3(void *item);

void list_unlink_by_id(List *l, col_i16 id)
{
    Item *p;

    if (l->head == 0)
        return;
    else {
        p = l->head;
        if (get_w_2_1060_abca(p) == id)
            l->head = (*((void * *)((unsigned char *)((Item *)p) + 0x12)));
        else
            while (p != 0) {
                if (get_p_12_1060_abe3(p) == 0)
                    goto done;
                if (get_w_2_1060_abca(get_p_12_1060_abe3(p)) == id) {
                    (*((void * *)((unsigned char *)(p) + 0x12))) = get_p_12_1060_abe3(get_p_12_1060_abe3(p));
                    goto done;
                }
                p = get_p_12_1060_abe3(p);
            }
    }
done:
    ;
}
