/* palette_load_file: candidate for 1068:0420 (323 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: palette_load_file
 *
 * Loads a palette range out of a file: two words of header -- the first index and the count --
 * then `count * 3` bytes of RGB into a locked handle, and `CTable_Fill` puts them in.
 * The handle is unlocked and disposed on the way out, so the file's copy does not survive the
 * call.
 *
 * `count * 3` is `MOV AX,[m] / ADD AX,AX / ADD AX,[m]`, Borland's strength reduction, and it is
 * what says the file holds RGB TRIPLES rather than the four-byte entries a Windows palette
 * uses.
 *
 * The file is opened before anything is allocated and a failure returns at once, which is why
 * there are TWO destructor calls for the one `DiskFile`: `-P` emits one per `return`.
 *
 * `DiskFile`'s constructor and destructor are reached through the five-byte NOP form, so they
 * are external to this translation unit even though they live in this segment (rule 47).
 */

#include "colonization.h"

typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */
#include "records.h"
class DiskFile { public: MSFile msfile; DiskFile(); DiskFile(char *name, int mode); ~DiskFile(); };

extern "C" int file_MSOpenFile_1068_95d5(DiskFile *file, const char *name);

/* same object: 4-byte `push cs; call ` (rule 47) */

struct logpal;
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

void palette_load_file(void *pal, char *name)
{
    DiskFile f;
    void *rgb;
    int first;
    int count;
    register short h;

    if (file_MSOpenFile_1068_95d5(&f, name) == 0)
        return;
    save_read_field_1068_95fb(&f, &first, 2L);
    save_read_field_1068_95fb(&f, &count, 2L);
    h = MSAllocateHandle((long)(count * 3));
    rgb = MSHLock(h);
    save_read_field_1068_95fb(&f, rgb, (long)(count * 3));
    MSCloseFile_1068_95b5(&f);
    CTable_Fill((CTable *)pal, first, count, (unsigned char *)rgb);
    rgb = MSHUnlock(h);
    MSDisposeHandle(h);
}

