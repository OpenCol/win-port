/* replace_with_copy: candidate for 1068:9393 (28 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `handle_replace_with_copy`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
extern void handle_replace_with_copy(void *p);

void replace_with_copy(void *p)
{
    handle_replace_with_copy((Handle *)p);
}
