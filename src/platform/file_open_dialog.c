/* file_open_dialog: candidate for 1070:8309 (46 bytes).  Verdict: RELOCATABLE.
 *
 * file_dialog(a, b, 0, d, 0): the open variant with no third pointer.
 */

#include "colonization.h"

int file_open_dialog(void *path, void *filter, int must_exist)
{
    return file_dialog((char *)path, (const char *)filter, 0, must_exist, 0);
}
