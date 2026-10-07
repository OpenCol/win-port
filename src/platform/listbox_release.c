/* listbox_release: candidate for 1070:5406 (14 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * The widget layer's release of a list box's window: empty in the shipped build, like
 * `image_button_release`. Its one caller checks the handle at +0x10 is non-zero
 * before calling it, which is what says the argument is a window handle rather than a count.
 */

#include "colonization.h"

void listbox_release(unsigned hwnd)
{
}
