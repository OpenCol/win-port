/* CTable_Scale: candidate for 1068:0abe (461 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CTable_Scale
 * name-source: sibling-build
 *
 * Step `step` of a fade over the table's range (+0x40a start, +0x40c count) between the
 * packed RGB source (handle at +0x41c) and the target colour (+0x414..+0x416): each channel
 * is src / steps * step + target / steps * (steps - step), summed as bytes (the source is a 
 * pointer: with a one the compiler evaluates the terms in the other order); then the range is
 * animated on the GDI palette. A step outside 0..steps (+0x408) is reported
 * ("Color Scale factor out of range").
 *
 * RENAMED 2026-09-26 from palette_fade: the Mac build's CTable::Scale(short): this plus a step, one step of the fade over the range CTable_SetScale recorded. It sits in the CTable module (1068:0000..0d94) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void report_error_1068_94ea(void *ctx, char *msg, long v);

#include "records.h"
extern void MSRawSetCTEntry(Palette *pal, int i, char r, char g, char b);
extern void MSAnimatePal(Palette *pal, unsigned hpal, int start, int count);

void CTable_Scale(CTable *table, int step)
{
    unsigned char r, g, b;
    unsigned char *src;
    int i;

    if (step < 0 || table->steps < step) {
        report_error_1068_94ea(&g_error_log, "Color Scale factor out of range", (long)step);
        return;
    }
    src = (unsigned char *)MSHLock(table->hsrc);
    for (i = 0; table->count * 3 > i; i += 3) {
        r = src[i] / table->steps * step + table->r / table->steps * (table->steps - step);
        g = src[i + 1] / table->steps * step + table->g / table->steps * (table->steps - step);
        b = src[i + 2] / table->steps * step + table->b / table->steps * (table->steps - step);
        MSRawSetCTEntry((Palette *)table, table->first + i / 3, r, g, b);
    }
    MSAnimatePal((Palette *)table, table->hpal, table->first, table->count);
    src = (unsigned char *)MSHUnlock(table->hsrc);
}
