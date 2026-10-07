/* MSOpenFile: candidate for 1070:7e76 (165 bytes).  Verdict: RELOCATABLE.
 *
 * Opens an existing file: OF_EXIST first (0 when absent), then OF_READWRITE, falling back to
 * OF_READ with the record marked read-only; 0 when even that fails.
 */

#include "colonization.h"

#include "records.h"

int MSOpenFile(const char *name, MSFile *file)
{
    if (OpenFile(name, file->ofs, 0x4000) != -1) {
        file->handle = OpenFile(name, file->ofs, 2);
        if (file->handle == -1) {
            file->writable = 0;
            file->handle = OpenFile(name, file->ofs, 0);
            if (file->handle == -1) {
                file->handle = 0;
                return 0;
            }
        } else {
            file->writable = 1;
        }
        return 1;
    } else {
        return 0;
    }
}
