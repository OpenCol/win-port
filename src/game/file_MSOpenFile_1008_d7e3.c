/* file_MSOpenFile_1008_d7e3: candidate for 1008:d7e3 (38 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's copy of the `MSOpenFile` wrapper, reaching segment 1070's copy rather than
 * the import directly: the two -pointer arguments swapped, as in every member of the
 * family.
 */

#include "colonization.h"

int file_MSOpenFile_1008_d7e3(void *file, void *name)
{
    return file_MSOpenFile_1070_7e50((char *)name, file);
}
