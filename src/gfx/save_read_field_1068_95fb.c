/* save_read_field_1068_95fb: candidate for 1068:95fb (42 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: save_read_field_1068_95fb
 *
 * Save-file wrapper: MSReadFile(f, buf, n) with the length as a long.
 *
 * TRANSFERRED: byte-identical to `save_read_field` (1008:d834) and reaching the same routine, one of
 * 12 copies of this function in the image.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern int MSReadFile(MSFile *file, void *buf, long n);

int save_read_field_1068_95fb(DiskFile *file, void *buf, long n)
{
    return MSReadFile(&file->msfile, buf, n);
}
