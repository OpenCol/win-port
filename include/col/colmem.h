/* col/colmem.h -- what the HOST test harness does to the segment image.
 *
 * Only the host build links this; the Win16 build has no use for it. The storage itself
 * is col/segments.h's -- the shipped data segments, lifted out of COLONIZE.EXE -- and
 * both targets address it identically. This adds the three operations a differential
 * needs and a real program does not: put the image back, mark a baseline, and say what
 * moved since.
 */
#ifndef COL_COLMEM_H
#define COL_COLMEM_H

#include "col/segments.h"

/* Restore every data segment to the image as it was extracted, so one test cannot see
 * what an earlier one wrote. The first call captures that image, so it must happen before
 * anything has written to a segment -- col_relocate() included. */
void col_reset(void);

void col_snapshot(void);                  /* mark the baseline for col_diff */

/* Every byte that differs from the baseline, as (segment, offset, value). Returns the
 * count, which may exceed `max`; the caller sizes the arrays. This is a DELTA, not a
 * trace: it cannot see a write of a value that was already there, nor the order writes
 * happened in. tests/differential.py says what that does and does not cover. */
int col_diff(int *segs, unsigned *offs, unsigned char *values, int max);

#endif
