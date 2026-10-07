/* wing_dib_flip: candidate for 1070:a736 (309 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Turns a DIB upside down in place: one row's worth of scratch is allocated and locked, and
 * the top and bottom rows are swapped through it, walking inward for half the height. A
 * three-call `hmemcpy` swap per row, and the scratch handle is disposed at the end.
 *
 * EVERY POINTER IS ``. The three moves are Borland's `ADD` / `ADC` / `SHL` by the imported
 * `__AHSHIFT` before the segment add, which is what a DIB needs: a 320x200 image is already
 * 64000 bytes and a `` pointer would wrap inside the segment on the way down.
 *
 * The row width at +0x10 is read TWO WAYS in the same function -- `MOVZX` for the allocation
 * size and `MOVSX` everywhere else. The field is a signed `short`; the allocation casts it to
 * `unsigned` first and then to the `unsigned long` the allocator takes.
 *
 * The multiply that finds the last row is `(long)w->rowbytes * (w->height - 1)` and not the
 * other order: Borland evaluates the left operand into DX:AX first, and the shipped loads
 * +0x10 there. `F_LXMUL` gets the two the other way round from the way they read.
 *
 * The loop test recomputes `w->height / 2` every iteration -- the height is re-read and
 * re-halved at the bottom of the loop, not hoisted.
 */

#include "colonization.h"

#include "records.h"
extern void *wing_dib_pointer(GrafRec *graf);

void wing_dib_flip(Wing *win)
{
    unsigned char *top;
    unsigned char *bot;
    unsigned char *tmp;
    unsigned char *base;
    register int y;
    register short h;

    base = (unsigned char *)wing_dib_pointer((GrafRec *)win);
    h = MSAllocateHandle((unsigned)win->rowbytes);
    tmp = (unsigned char *)MSHLock(h);
    top = base;
    bot = base + (long)win->rowbytes * (win->height - 1);
    for (y = 0; win->height / 2 > y; y++) {
        hmemcpy(tmp, bot, (long)win->rowbytes);
        hmemcpy(bot, top, (long)win->rowbytes);
        hmemcpy(top, tmp, (long)win->rowbytes);
        top += win->rowbytes;
        bot -= win->rowbytes;
    }
    MSDisposeHandle(h);
}
