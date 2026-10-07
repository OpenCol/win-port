/* copy_all_to_screen_1050_843b: candidate for 1050:843b (30 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Copies the WHOLE object to the screen: this module's `copy_rect_to_screen_1050_8bfa` with a NULL
 * rectangle, which the 32-bit `66 6A 00` push is -- one NULL, not two zero words.
 * One of THIRTEEN copies, one per module.
 *
 * The callee is -called and is a stub here (rule 47).
 */

#include "colonization.h"

#include "classes.h"

#include "w16types.h"
/* same object: 4-byte `push cs; call ` */
extern void copy_rect_to_screen_1050_8bfa(GameWin *w, RECT *r);

void copy_all_to_screen_1050_843b(GameWin *port)
{
    copy_rect_to_screen_1050_8bfa(port, 0);
}
