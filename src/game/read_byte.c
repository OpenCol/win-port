/* PORT: this file's own declarations of read_file_1040_a84d removed: the header declares it from the definition (W1 step 3). */
/* read_byte: candidate for 1040:60fb (70 bytes).  Verdict: RELOCATABLE.
 * symbol: read_byte
 *
 * Reads one byte from the file and seeks it (MSSeekFile_1040_a877 with 0xffff).
 * Both helpers are -called and defined in-file as stubs.
 */

#include "colonization.h"

#include "records.h"
extern int g_stub_a;


char read_byte(void *file)
{
    char byte;

    read_file_1040_a84d((MSFile *)file, &byte, 1L);
    MSSeekFile_1040_a877(file, 0xffffL);
    return byte;
}
