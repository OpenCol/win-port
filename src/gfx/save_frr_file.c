/* save_frr_file: candidate for 1068:2a7a (357 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: save_frr_file
 *
 * Writes an object's rectangle cache next to its file: the name at +0x132 with its last four
 * characters replaced by ".frr" (DGROUP:0x2bf3). The file holds the four bytes at +0x3c, the
 * count at +0x568, `count` words from the pointer at +0x560 and `count` 8-byte rectangles
 * from the one at +0x55c. The answer is 0 when the file cannot be created and 1 when it was
 * written. Its caller hands "Total rects calculated:" (DGROUP:0x2bdb) to the empty debug routine
 * `report_error_1068_94ea` just before.
 *
 * The two sizes are `unsigned` products widened to `long` -- `MOVZX EAX,AX` -- and the doubling
 * is `ADD AX,AX` where the times eight is `SHL AX,3`.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

/* same object: 4-byte `push cs; call ` (rule 47) */

char save_frr_file(Flic *flic)
{
    DiskFile f;
    char name[12];

    name[0] = 0;
    L1_0c72(name, flic->name, STRLEN(flic->name) - 4);
    STRCAT(name, ".frr");
    if (MSCreateFile_1068_9565(&f, name) == 0)
        return 0;
    MSWriteFile_1068_958b(&f, &flic->created, 4L);
    MSWriteFile_1068_958b(&f, &flic->count, 2L);
    MSWriteFile_1068_958b(&f, flic->rect_frames, (long)(flic->count * 2));
    MSWriteFile_1068_958b(&f, flic->rects, (long)(flic->count * 8));
    MSCloseFile_1068_95b5(&f);
    return 1;
}

