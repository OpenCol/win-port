/* MSGetResource_1068_94c7: candidate for 1068:94c7 (35 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSGetResource`, named for the routine it
 * reaches: the four-character type as a pointer and the resource id. One of FOUR
 * copies.
 *
 * 35 bytes, not the census 31: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is a call and is `extern`.
 *
 * RENAMED 2026-09-27 from MSGetNamedResource_1068_94c7: the Mac build's MSGetResource(unsigned char*, unsigned short): the body takes (type4, int id) and hands FindResource an integer resource (0:id); the by-NAME getter, MSGetNamedResource(uchar*, uchar*), is 1078:0277 (renamed with it).
 */

#include "colonization.h"

extern void *MSGetResource(char *type4, int id);

void *MSGetResource_1068_94c7(void *type, int id)
{
    return MSGetResource((char *)type, id);
}
