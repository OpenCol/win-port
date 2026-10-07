/* resource_find_1050_8d9c: candidate for 1050:8d9c (38 bytes).  Verdict: RELOCATABLE.
 *
 * MSGetNamedResource with the key pointer and name passed through.
 */

#include "colonization.h"

void *resource_find_1050_8d9c(long *key, char *name)
{
    return MSGetNamedResource((unsigned char *)key, name);
}
