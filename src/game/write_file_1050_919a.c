/* write_file_1050_919a: candidate for 1050:919a (42 bytes).  Verdict: RELOCATABLE.
 *
 * MSWriteFile with the arguments passed straight through.
 */

#include "colonization.h"

#include "records.h"
extern int MSWriteFile(void *file, void *buf, long n);

int write_file_1050_919a(void *file, void *buf, long n)
{
    return MSWriteFile((MSFile *)file, buf, n);
}
