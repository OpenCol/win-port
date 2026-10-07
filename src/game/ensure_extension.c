/* ensure_extension: candidate for 1040:6773 (101 bytes).  Verdict: RELOCATABLE.
 *
 * Appends "." and the extension to a file name that has no dot, then
 * upper-cases the whole name.
 */

#include "colonization.h"

void ensure_extension(char *path, char *ext)
{
    char *dot;

    dot = strrchr_rtl(path, '.');
    if (dot == 0) {
        STRCAT(path, ".");
        STRCAT(path, ext);
    }
    STRUPR(path);
}
