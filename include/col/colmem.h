/* col/colmem.h -- the host build's memory, in the game's own coordinates.
 *
 * Only the host (COL_HOST) uses this. The Win16 build lets the linker place its data and
 * never asks where anything landed.
 *
 * WHY THE HOST PLACES DATA AT THE SHIPPED ADDRESSES. The oracle for a ported function is
 * the SHIPPED function, run in the byte-exact repo's emulator, which reports every read
 * and write as a `seg:off` in the program's own coordinates. If the host put a global
 * wherever the loader felt like, no comparison would be possible without a name-to-
 * address map applied to every observation. Placing the host's storage at the shipped
 * address instead makes the two coordinate systems the same one, and the comparison is
 * then a straight diff. It costs one array per segment.
 */
#ifndef COL_COLMEM_H
#define COL_COLMEM_H

#define COL_SEG_MAX 35

unsigned char *col_seg(int seg);          /* the slab for one shipped segment */
void col_reset(void);                     /* zero every slab, forget every snapshot */
void col_snapshot(void);                  /* remember the slabs as they are now */

/* Every byte that changed since col_snapshot(), as `seg:off=value` triples. Returns the
 * count; the caller sizes the arrays. This is the host's answer to the emulator's write
 * log -- a delta, not a trace: it cannot see a write that was overwritten, nor the order
 * they happened in. tests/README.md says what that does and does not cover. */
int col_diff(int *segs, unsigned *offs, unsigned char *values, int max);

#endif
