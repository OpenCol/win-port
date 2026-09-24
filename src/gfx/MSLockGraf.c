/* MSLockGraf -- shipped at 1068:4ed3.
 *
 * Ported by tools/port.py from matched/gfx/MSLockGraf.c.
 *
 * The file's own Win, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * wing_dib_pointer(Win *w);
extern col_i16 is_state_one(Win *w);
extern void * __bnew_qui_rtl(col_u16 n); /* operator new(unsigned) */

void MSLockGraf(GRPort *p)
{
    register col_i16 y;

    if ((*((void * *)((unsigned char *)(p) + 0x18))) == 0) {
        (*((void * *)((unsigned char *)(p) + 0x18))) = wing_dib_pointer((*((Win * *)((unsigned char *)(p) + 0x22))));
        if ((*((void * *)((unsigned char *)(p) + 0x18))) != 0) {
            (*((col_i32 * *)((unsigned char *)(p) + 0x1c))) = (col_i32 *)__bnew_qui_rtl((*((col_i16 *)((unsigned char *)(p) + 0x4))) * 4);
            if (is_state_one((*((Win * *)((unsigned char *)(p) + 0x22)))) == 0) {
                ((col_i32 * *)((unsigned char *)(p) + 0x1c))[0] = (col_i32)((*((col_i16 *)((unsigned char *)(p) + 0x4))) - 1) * (*((col_i16 *)((unsigned char *)(p) + 0x6)));
                for (y = 1; (*((col_i16 *)((unsigned char *)(p) + 0x4))) > y; y++)
                    ((col_i32 * *)((unsigned char *)(p) + 0x1c))[y] = ((col_i32 * *)((unsigned char *)(p) + 0x1c))[y - 1] - (*((col_i16 *)((unsigned char *)(p) + 0x6)));
            } else {
                ((col_i32 * *)((unsigned char *)(p) + 0x1c))[0] = 0;
                for (y = 1; (*((col_i16 *)((unsigned char *)(p) + 0x4))) > y; y++)
                    ((col_i32 * *)((unsigned char *)(p) + 0x1c))[y] = ((col_i32 * *)((unsigned char *)(p) + 0x1c))[y - 1] + (*((col_i16 *)((unsigned char *)(p) + 0x6)));
            }
        }
    }
}
