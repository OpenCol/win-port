/* MSCreateFile_1040_aa10: candidate for 1040:aa10 (38 bytes).  Verdict: RELOCATABLE.
 *
 * MSCreateFile with its two -pointer arguments swapped.
 *
 * TRANSFERRED: byte-identical to `file_MSCreateFile_1050_9174` (1050:9174) and reaching the same routine, one of
 * 11 copies of this function in the image.
 *
 * TRANSFERRED from `file_MSOpenFile_1038_e492` (1038:e492), byte-identical except for the one relocated call:
 * this copy reaches `MSCreateFile` where that one reaches `MSOpenFile`. The comparator masks that
 * word, so the name comes from the disassembly and not from the verdict.
 */

#include "colonization.h"

#include "records.h"
extern int MSCreateFile(const char *name, MSFile *file);

int MSCreateFile_1040_aa10(void *file, void *name)
{
    return MSCreateFile((const char *)name, (MSFile *)file);
}
