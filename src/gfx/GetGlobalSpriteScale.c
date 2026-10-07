/* GetGlobalSpriteScale: candidate for 1068:65e8 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Returns the pair of words at DGROUP:0x2b0e and 0x2b10 through two pointers.
 *
 * RENAMED 2026-09-26 from get_scale_pair: the Mac build's GetGlobalSpriteScale(short*, short*), bound by call-sequence alignment (1.0 aligned gaps in ColChild_DrawTitleBar leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.81x this one.
 */

#include "colonization.h"

void GetGlobalSpriteScale(int *num, int *den)
{
    *num = g_scale_num;
    *den = g_scale_den;
}
