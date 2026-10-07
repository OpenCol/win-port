/* MSCreateFile: candidate for 1070:7f47 (89 bytes).  Verdict: RELOCATABLE.
 *
 * OpenFile(name, &f->ofs, OF_CREATE | OF_READWRITE); a failed open clears the record and
 * returns 0, else the file is marked writable and 1 returned.
 */

#include "colonization.h"

#include "records.h"

int MSCreateFile(const char *name, MSFile *file)
{
    file->handle = OpenFile(name, file->ofs, 0x1002);
    if (file->handle == -1) {
        file->handle = 0;
        file->writable = 0;
        return 0;
    } else {
        file->writable = 1;
        return 1;
    }
}
