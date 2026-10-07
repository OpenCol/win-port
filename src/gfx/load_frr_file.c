/* load_frr_file: candidate for 1068:2bfc (833 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: load_frr_file
 *
 * Reads back the rectangle cache `save_frr_file` writes. When the object names a
 * resource (+0x236 not -1) the cache is taken from that resource, of the type at DGROUP:0x2afa;
 * otherwise from the file named at +0x132 with its last four characters replaced by ".frr"
 * (DGROUP:0x2bf8). Either way it holds four bytes that must equal the long at +0x3c, a word
 * count, `count` words copied to the pointer at +0x560 and `count` 8-byte rectangles to the
 * one at +0x55c. Its one caller, 1068:2785, returns at once when the answer is nonzero.
 *
 * The file path answers 0 when the file is missing or its first four bytes differ, and 1 when read.
 * The resource path answers 1 in every case -- a missing resource and a mismatched one too -- so a
 * resource-backed object whose cache is stale is never rebuilt. The count is read into a local and
 * not stored in the object.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void *MSGetResource_1068_94c7(void *type, int id);
extern "C" void *MSLockResource_1068_94f8(void *h);
extern "C" void MSUnlockResource_1068_952d(void *res);
extern "C" void MSReleaseResource_1068_9549(void *res);
extern "C" int file_MSOpenFile_1068_95d5(DiskFile *file, char *name);

char load_frr_file(Flic *flic)
{
    DiskFile f;
    unsigned count;
    long magic;
    void *res;
    long type;
    unsigned char *data;
    char name[12];

    if (flic->res_id != -1) {
        type = g_frr_resource_type;
        res = MSGetResource_1068_94c7(&type, flic->res_id);
        if (res != 0) {
            data = (unsigned char *)MSLockResource_1068_94f8(res);
            MSCopyMem(data, &magic, 4L);
            data += 4;
            if (flic->created == magic) {
                MSCopyMem(data, &count, 2L);
                data += 2;
                MSCopyMem(data, flic->rect_frames, (long)(count * 2));
                data += (long)(count * 2);
                MSCopyMem(data, flic->rects, (long)(count * 8));
                MSUnlockResource_1068_952d(res);
                data = 0;
                MSReleaseResource_1068_9549(res);
                return 1;
            }
            MSUnlockResource_1068_952d(res);
            data = 0;
            MSReleaseResource_1068_9549(res);
        }
    } else {
        name[0] = 0;
        L1_0c72(name, flic->name, STRLEN(flic->name) - 4);
        STRCAT(name, ".frr");
        if (file_MSOpenFile_1068_95d5(&f, name) == 0)
            return 0;
        save_read_field_1068_95fb(&f, &magic, 4L);
        if (flic->created != magic)
            return 0;
        save_read_field_1068_95fb(&f, &count, 2L);
        save_read_field_1068_95fb(&f, flic->rect_frames, (long)(count * 2));
        save_read_field_1068_95fb(&f, flic->rects, (long)(count * 8));
        return 1;
    }
    return 1;
}

