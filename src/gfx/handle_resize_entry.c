/* handle_resize_entry: candidate for 1068:9467 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `handle_resize(r, size)`.
 *
 * A pure pass-through: every argument goes on unchanged. Nothing in the image calls it, so
 * why the program carries a second symbol for the same operation is not shown -- the name
 * says only that this is a separate entry onto `handle_resize`.
 *
 * handle_resize is in the widget segment, a call, so it is extern.
 */

#include "colonization.h"

extern void handle_resize(short *hp, long size);

void handle_resize_entry(short *hp, long size)
{
    handle_resize(hp, size);
}
