/* PORT: this file's own declarations of handle_replace_with_copy removed: the header declares it from the definition (W1 step 3). */
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

void replace_with_copy(void *p)
{
    handle_replace_with_copy((Handle *)p);
}
