/* image_button_release: candidate for 1070:2774 (14 bytes).  Verdict: EXACT.
 *
 * The widget layer's release of an image button's handle: empty in the shipped build
 * (called from SImageButton's destructor).
 */

#include "colonization.h"

void image_button_release(unsigned h)
{
}
