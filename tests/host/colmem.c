/* colmem.c -- see col/colmem.h. Host build only.
 *
 * The segments are not contiguous and not all the same size, so both copies are kept as
 * one flat buffer with an index, rather than as a 35 x 64 KiB block: the data is 123 KB
 * and most of the 2.2 MB that would cost is segments that do not exist.
 */
#include <stdlib.h>
#include <string.h>
#include "col/colmem.h"

#define MAXSEG 64

static unsigned char *pristine;           /* the image as extracted, captured once */
static unsigned char *baseline;           /* whatever col_snapshot() last saw */
static long at[MAXSEG];                   /* segment -> offset into those buffers */
static long total;
static int ids[MAXSEG], nseg;

static void index_segments(void)
{
    int s;
    if (nseg)
        return;
    for (s = 0; s < MAXSEG; s++)
        if (col_seg(s)) {
            at[s] = total;
            total += col_seg_size(s);
            ids[nseg++] = s;
        }
    pristine = (unsigned char *)malloc((size_t)total);
    baseline = (unsigned char *)malloc((size_t)total);
    for (s = 0; s < nseg; s++)
        memcpy(pristine + at[ids[s]], col_seg(ids[s]), (size_t)col_seg_size(ids[s]));
    memcpy(baseline, pristine, (size_t)total);
}

void col_capture(void)
{
    int i;
    nseg = 0;
    index_segments();
    for (i = 0; i < nseg; i++)
        memcpy(pristine + at[ids[i]], col_seg(ids[i]), (size_t)col_seg_size(ids[i]));
}


void col_reset(void)
{
    int i;
    index_segments();
    for (i = 0; i < nseg; i++)
        memcpy(col_seg(ids[i]), pristine + at[ids[i]], (size_t)col_seg_size(ids[i]));
}

void col_snapshot(void)
{
    int i;
    index_segments();
    for (i = 0; i < nseg; i++)
        memcpy(baseline + at[ids[i]], col_seg(ids[i]), (size_t)col_seg_size(ids[i]));
}

int col_diff(int *segs, unsigned *offs, unsigned char *values, int max)
{
    int i, n = 0;
    long off, size;
    index_segments();
    for (i = 0; i < nseg; i++) {
        unsigned char *now = col_seg(ids[i]);
        unsigned char *was = baseline + at[ids[i]];
        size = col_seg_size(ids[i]);
        for (off = 0; off < size; off++)
            if (now[off] != was[off]) {
                if (n < max) {
                    segs[n] = ids[i];
                    offs[n] = (unsigned)off;
                    values[n] = now[off];
                }
                n++;
            }
    }
    return n;
}
