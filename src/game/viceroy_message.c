/* viceroy_message: candidate for 1038:b20f (43 bytes).  Verdict: RELOCATABLE.
 *
 * Appends the text number at `idx` in the MISC word table (SEG20:0xdcc8) to the
 * map window record at SEG20:0x658.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void ColChild_AddToMessage(ColChild *child, int idx);

void viceroy_message(int idx)
{
    ColChild_AddToMessage((ColChild *)&g_map_window, g_misc_words[idx]);
}
