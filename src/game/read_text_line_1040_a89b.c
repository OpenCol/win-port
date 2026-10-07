/* read_text_line_1040_a89b: candidate for 1040:a89b (41 bytes).  Verdict: RELOCATABLE.
 *
 * read_text_line with the three arguments (file, out, max) passed through.
 */

#include "colonization.h"

#include "records.h"
extern int read_text_line(void *file, char *out, int max);

int read_text_line_1040_a89b(void *file, char *out, int max)
{
    return read_text_line((MSFile *)file, out, max);
}
