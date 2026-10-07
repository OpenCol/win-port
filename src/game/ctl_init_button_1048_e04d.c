/* ctl_init_button_1048_e04d: candidate for 1048:e04d (58 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `ctl_init_button_1048_e831` with one argument taken out of the list and stored instead:
 * the word at BP+0x14 goes to +0x20 of the control record and every other argument is passed
 * straight through. Ten words in, nine words on.
 *
 * That one dropped argument is the whole of this function, and it is why it exists: the
 * caller supplies a field the initialiser does not take.
 *
 * The trailing four bytes are the button's TITLE, not a width/height pair -- a split the
 * comparator cannot see (both total four), settled by `EuropeWin_Init`, which calls this
 * with "~Recruit", "~Purchase" and "~Train", and by the `MSInitButton(r, c, title, 1)` at
 * the end of the chain.
 *
 * The callee is -called and is a stub here (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */
extern void ctl_init_button_1048_e831(SButton *button, void *win, int id, RECT *r, char *title);

void ctl_init_button_1048_e04d(SButton *button, void *win, int id, RECT *r,
                                   int colour, char *title)
{
    button->colour = colour;
    ctl_init_button_1048_e831(button, win, id, r, title);
}
