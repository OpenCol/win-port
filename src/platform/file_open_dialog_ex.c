/* file_open_dialog_ex: candidate for 1070:8337 (49 bytes).  Verdict: RELOCATABLE.
 *
 * file_dialog(a, b, c, d, 0): the open variant.
 */

#include "colonization.h"

int file_open_dialog_ex(void *path, void *filter, void *ext, int must_exist)
{
    return file_dialog((char *)path, (const char *)filter, (const char *)ext, must_exist, 0);
}
