/* resource_find_1040_a976: candidate for 1040:a976 (38 bytes).  Verdict: RELOCATABLE.
 *
 * MSGetNamedResource with the key pointer and name passed through.
 *
 * TRANSFERRED: byte-identical to `resource_find_1050_8d9c` (1050:8d9c) and reaching the same routine, one of
 * 4 copies of this function in the image.
 */

#include "colonization.h"

void *resource_find_1040_a976(long *key, char *name)
{
    return MSGetNamedResource((unsigned char *)key, name);
}
