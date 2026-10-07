/* file_MSCreateFile_1008_d89e: candidate for 1008:d89e (38 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `file_MSOpenFile_1008_d7e3`'s twin for creation rather than opening: the same two 
 * pointers, swapped, into segment 1070's `MSCreateFile` wrapper.
 */

#include "colonization.h"

int file_MSCreateFile_1008_d89e(void *file, void *name)
{
    return file_MSCreateFile_1070_7f21((char *)name, file);
}
