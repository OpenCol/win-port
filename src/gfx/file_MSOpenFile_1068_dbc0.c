/* file_MSOpenFile_1068_dbc0: candidate for 1068:dbc0 (38 bytes).  Verdict: RELOCATABLE.
 *
 * MSOpenFile with its two -pointer arguments swapped.
 *
 * TRANSFERRED: byte-identical to `file_MSOpenFile_1050_9096` (1050:9096) and reaching the same routine, one of
 * 11 copies of this function in the image.
 *
 * The two pointers are the FILE and the NAME, in that order, swapped on the way into
 * `MSOpenFile` -- whose own reconstruction takes the name first. Every caller passes
 * `&f` then a string.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: file_MSOpenFile_1068_dbc0
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern int MSOpenFile(const char *name, MSFile *file);

int file_MSOpenFile_1068_dbc0(DiskFile *file, const char *name)
{
    return MSOpenFile(name, &file->msfile);
}
