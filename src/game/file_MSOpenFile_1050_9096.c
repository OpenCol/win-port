/* file_MSOpenFile_1050_9096: candidate for 1050:9096 (38 bytes).  Verdict: RELOCATABLE.
 *
 * MSOpenFile with its two -pointer arguments swapped.
 *
 * The two pointers are the FILE and the NAME, in that order, swapped on the way into
 * `MSOpenFile` -- whose own reconstruction takes the name first. Every caller passes
 * `&f` then a string.
 */

#include "colonization.h"

#include "records.h"
extern int MSOpenFile(const char *name, MSFile *file);

int file_MSOpenFile_1050_9096(MSFile *file, const char *name)
{
    return MSOpenFile(name, file);
}
