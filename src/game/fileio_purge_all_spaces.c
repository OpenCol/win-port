/* fileio_purge_all_spaces: candidate for 1040:6a12 (120 bytes, 2 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * trim_right, then skip leading blanks and copy the rest back over s through a buffer.
 *
 * RENAMED 2026-09-26 from trim_both: the Mac build's fileio_purge_all_spaces(char*), bound by call-graph agreement (1 of its Mac call edges already bound land here, runner-up 0) and by call-sequence alignment (the lone unbound callee of its gap in text_item_get); the arity agrees and the Mac body is 1.25x this one.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void fileio_purge_all_spaces(char *s)
{
    char *p;
    char buf[0x100];

    trim_right(s);
    p = s;
    while (*p != 0 && ((int)*p == ' ' || (int)*p == '\t'))
        p++;
    STRCPY(buf, p);
    STRCPY(s, buf);
}
