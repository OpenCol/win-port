/* read_file_1040_a84d: candidate for 1040:a84d (42 bytes).  Verdict: RELOCATABLE.
 *
 * MSReadFile with the arguments passed straight through.
 *
 * The three trailing words are `(buf, n)` -- a pointer and a `long` -- and not four
 * separate ints; the totals agree either way, so the comparator cannot tell (§119).
 * `MSReadFile`'s own reconstruction settles it, and `GamePrefs_LoadPrefs` already calls
 * this module's twin that way.
 */

#include "colonization.h"

#include "records.h"
extern int MSReadFile(MSFile *file, void *buf, long n);

int read_file_1040_a84d(MSFile *file, void *buf, long n)
{
    return MSReadFile(file, buf, n);
}
