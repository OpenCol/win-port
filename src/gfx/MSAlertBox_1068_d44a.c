/* MSAlertBox_1068_d44a: candidate for 1068:d44a (38 bytes).  Verdict: RELOCATABLE.
 *
 * MSAlertBox with the text pointer, icon and button set passed through.
 *
 * TRANSFERRED: byte-identical to `resource_find_1050_8d9c` (1050:8d9c) except for the relocated call, which reaches a different routine; one of
 * 4 copies of this function in the image.
 *
 * TRANSFERRED from `resource_find_1040_a976` (1040:a976), byte-identical except for the one relocated call:
 * this copy reaches `MSAlertBox` where that one reaches `MSGetNamedResource`. The comparator masks that
 * word, so the name comes from the disassembly and not from the verdict.
 */

#include "colonization.h"

int MSAlertBox_1068_d44a(char *text, int icon, int buttons)
{
    return MSAlertBox(text, icon, buttons);
}
