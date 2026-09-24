/* MSUnlockGraf -- shipped at 1068:5010.
 *
 * Ported by tools/port.py from matched/gfx/MSUnlockGraf.c.
 *
 * The file's own MSWin, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * return_null(MSWin *w);
extern void __bdele_qnv_rtl(void *p); /* operator delete(void*) */

void MSUnlockGraf(GRPort *p)
{
    if ((*((void * *)((unsigned char *)(p) + 0x18))) != 0) {
        (*((void * *)((unsigned char *)(p) + 0x18))) = return_null((*((MSWin * *)((unsigned char *)(p) + 0x22))));
        __bdele_qnv_rtl((*((col_i32 * *)((unsigned char *)(p) + 0x1c))));
    }
}
