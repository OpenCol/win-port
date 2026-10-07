/* flic_load_resource2: candidate for 1068:22df (52 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: flic_load_resource2
 *
 * Loads a FLIC resource with a given PALETTE RANGE: the first entry and count at +0x552/+0x554
 * are set and flic_load_resource does the rest; it and the module's resource wrappers are
 * defined here because the shipped calls to them are the compiler's own `push cs / call `.
 *
 * (Corrected 2026-09-26: this said a playback WIDTH and HEIGHT. The only caller, ClosingWin_ctor,
 * passes 10 and 0xec -- palette entries 10..245, the 236 a Windows 256-colour palette leaves once its
 * 20 static colours are reserved -- and flic_seek_frame reads +0x552/+0x554 as the palette
 * range it installs after a colour chunk.)
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

extern void *MSGetResource(char *type, int id);
extern void *MSLockResource(void *h);

extern void Flic_NextFrame(Flic *flic);

extern "C" void *MSGetResource_1068_94c7(void *type, int id);

extern "C" void report_error_1068_94ea(void *ctx, char *msg, long v);

extern "C" void *MSLockResource_1068_94f8(void *h);

extern "C" void flic_load_resource(Flic *flic, int id);

void flic_load_resource2(Flic *flic, int id, int first, int count)
{
    flic->pal_first = first;
    flic->pal_count = count;
    flic_load_resource(flic, id);
}

