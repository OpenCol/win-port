/* palette_save_file: candidate for 1068:02c8 (315 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: palette_save_file
 *
 * `palette_load_file`'s inverse, 0x158 bytes above it and the same file format read
 * backwards: `count * 3` bytes are packed out of the palette, then the first index, the count
 * and the RGB are written in that order.
 *
 * The buffer is allocated and FILLED BEFORE the file is created, so a file that will not open
 * leaks the handle -- the early return skips both the unlock and the dispose. The destructor
 * still runs, because `-P` emits one per `return`.
 *
 * `&first` and `&count` are the addresses of PARAMETERS, written straight out of the frame.
 *
 * Same `MOV AX,[m] / ADD AX,AX / ADD AX,[m]` for the size, twice over, and it says the same
 * thing it does in the loader: three bytes to the entry, RGB triples.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

struct logpal;
extern void MSCopyRawColors(struct logpal *pal, unsigned char *buf, int start, int count);
/* same object: 4-byte `push cs; call ` (rule 47) */

void palette_save_file(void *pal, char *name, int first, int count)
{
    DiskFile f;
    void *rgb;
    register short h;

    h = MSAllocateHandle((long)(count * 3));
    rgb = MSHLock(h);
    MSCopyRawColors((struct logpal *)pal, (unsigned char *)rgb, first, count);
    if (MSCreateFile_1068_9565(&f, name) == 0)
        return;
    MSWriteFile_1068_958b(&f, &first, 2L);
    MSWriteFile_1068_958b(&f, &count, 2L);
    MSWriteFile_1068_958b(&f, rgb, (long)(count * 3));
    MSCloseFile_1068_95b5(&f);
    rgb = MSHUnlock(h);
    MSDisposeHandle(h);
}

