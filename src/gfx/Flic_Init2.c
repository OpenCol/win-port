/* PORT: this file's own declarations of MSOpenFile removed: the header declares it from the definition (W1 step 3). */
/* Flic_Init2: candidate for 1068:2313 (55 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Flic_Init2
 *
 * Opens a FLIC animation with a given PALETTE RANGE: the first entry and count at +0x552/+0x554
 * are set and Flic_Init does the rest. Flic_Init and the module's file wrappers are defined
 * here because the shipped calls to them are the compiler's own `push cs / call `.
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

extern int MSReadFile(MSFile *file, void *buf, long n);
extern int MSSeekFile(MSFile *file, long pos);

extern void Flic_NextFrame(Flic *flic);

extern "C" int file_MSOpenFile_1068_95d5(DiskFile *file, char *name);

extern "C" void report_error_1068_963d(void *ctx, char *msg);

extern "C" void Flic_Init(Flic *flic, char *name);

void Flic_Init2(Flic *flic, char *name, int first, int count)
{
    flic->pal_first = first;
    flic->pal_count = count;
    Flic_Init(flic, name);
}

