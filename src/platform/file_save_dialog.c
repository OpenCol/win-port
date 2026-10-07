/* file_save_dialog: candidate for 1070:8368 (49 bytes).  Verdict: RELOCATABLE.
 *
 * file_dialog(a, b, c, d, 1): the save variant.
 */

#include "colonization.h"

int file_save_dialog(void *path, void *filter, void *ext, int must_exist)
{
    return file_dialog((char *)path, (const char *)filter, (const char *)ext, must_exist, 1);
}
