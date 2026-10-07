/* PORT: this file's own declarations of MSWriteFile removed: the header declares it from the definition (W1 step 3). */
/* write_file_1040_aa36: candidate for 1040:aa36 (42 bytes).  Verdict: RELOCATABLE.
 *
 * MSWriteFile with the arguments passed straight through.
 */

#include "colonization.h"

#include "records.h"

int write_file_1040_aa36(void *file, void *buf, long n)
{
    return MSWriteFile((MSFile *)file, buf, n);
}
