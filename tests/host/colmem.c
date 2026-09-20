/* colmem.c -- the host build's slabs. See col/colmem.h for why they exist.
 *
 * One 64 KiB array per shipped segment, which is what a 16-bit segment is. 35 of them is
 * 2.2 MB of BSS, which costs nothing on a host and keeps every address in the model
 * directly indexable with no bookkeeping.
 */
#include <string.h>
#include "col/colmem.h"

#define SEG_SIZE 0x10000L

static unsigned char slab[COL_SEG_MAX][SEG_SIZE];
static unsigned char shot[COL_SEG_MAX][SEG_SIZE];

unsigned char *col_seg(int seg)
{
    return slab[seg];
}

void col_reset(void)
{
    memset(slab, 0, sizeof slab);
    memset(shot, 0, sizeof shot);
}

void col_snapshot(void)
{
    memcpy(shot, slab, sizeof slab);
}

int col_diff(int *segs, unsigned *offs, unsigned char *values, int max)
{
    int seg, n = 0;
    long off;
    for (seg = 0; seg < COL_SEG_MAX; seg++)
        for (off = 0; off < SEG_SIZE; off++)
            if (slab[seg][off] != shot[seg][off]) {
                if (n < max) {
                    segs[n] = seg;
                    offs[n] = (unsigned)off;
                    values[n] = slab[seg][off];
                }
                n++;
            }
    return n;
}
