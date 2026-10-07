/* file_MSOpenFile_1038_e492: candidate for 1038:e492 (38 bytes).  Verdict: RELOCATABLE.
 *
 * MSOpenFile with its two -pointer arguments swapped.
 *
 * TRANSFERRED: byte-identical to `file_MSOpenFile_1050_9096` (1050:9096) and reaching the same routine, one of
 * 11 copies of this function in the image.
 *
 * The two pointers are the FILE and the NAME, in that order, swapped on the way into
 * `MSOpenFile` -- whose own reconstruction takes the name first. Every caller passes
 * `&f` then a string.
 */

#include "colonization.h"

#include "records.h"
extern int MSOpenFile(const char *name, MSFile *file);

int file_MSOpenFile_1038_e492(MSFile *file, const char *name)
{
    return MSOpenFile(name, file);
}
