/* list_find_by_id: candidate for 1070:8f5a (104 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * Walks a linked list of controls from p for the one whose window handle (+0x10, ctl_hwnd_1070_9681) equals hwnd; NULL when none does.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int ctl_hwnd_1070_9681(void *item);
extern void *ctl_next_1070_a09a(void *item);
void * list_find_by_id(int hwnd, void *p)
{
    int n = hwnd;
    void *q = p;

    while (q != 0) {
        if (ctl_hwnd_1070_9681((DlgControl *)q) == n)
            return q;
        q = ctl_next_1070_a09a((DlgControl *)q);
    }
    return 0;
}
