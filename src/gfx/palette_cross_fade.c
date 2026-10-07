/* palette_cross_fade: candidate for 1068:0d94 (549 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: palette_cross_fade
 *
 * Step `step` of a cross-fade over the table's range (+0x410 start, +0x412 count) between
 * two packed RGB sources (handles +0x418 and +0x41a): each channel is a / steps * step +
 * b / steps * (steps - step), summed as bytes; then the range is animated on the GDI palette
 * and both handles unlocked.
 * A step outside 0..steps2 (+0x40e) is reported ("Color Scale factor out of range").
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void report_error_1068_94ea(void *ctx, char *msg, long v);

#include "records.h"
extern void MSRawSetCTEntry(Palette *pal, int i, char r, char g, char b);
extern void MSAnimatePal(Palette *pal, unsigned hpal, int start, int count);

void palette_cross_fade(CTable *table, int step)
{
    unsigned char r, g, b;
    unsigned char *a;
    unsigned char *bs;
    int i;

    if (step < 0 || table->steps2 < step) {
        report_error_1068_94ea(&g_error_log, "Color Scale factor out of range", (long)step);
        return;
    }
    a = (unsigned char *)MSHLock(table->hsrc_a);
    bs = (unsigned char *)MSHLock(table->hsrc_b);
    for (i = 0; table->count2 * 3 > i; i += 3) {
        r = a[i] / table->steps2 * step + bs[i] / table->steps2 * (table->steps2 - step);
        g = a[i + 1] / table->steps2 * step + bs[i + 1] / table->steps2 * (table->steps2 - step);
        b = a[i + 2] / table->steps2 * step + bs[i + 2] / table->steps2 * (table->steps2 - step);
        MSRawSetCTEntry((Palette *)table, table->first2 + i / 3, r, g, b);
    }
    MSAnimatePal((Palette *)table, table->hpal, table->first2, table->count2);
    a = (unsigned char *)MSHUnlock(table->hsrc_a);
    bs = (unsigned char *)MSHUnlock(table->hsrc_b);
}
