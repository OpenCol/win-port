/* EuropeWin_EuropeButtonClick: candidate for 1048:8901 (121 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeButtonClick
 * name-source: sibling-build
 *
 * Three menu commands of the Europe screen: 200 opens `europe_recruit`, 201 is
 * `europe_purchase` and 202 is `europe_recruit_any`. Anything else does nothing.
 *
 * Its address sits three times in a handler table at SEG10:0x8809, 0x8857 and 0x88a5, which
 * is what says it is a registered command handler rather than a helper.
 *
 * THE WINDOW POINTER IS WALKED BACK 0x26 BYTES. `GetEventWindow` returns a member and this
 * subtracts 0x26 to reach the object that contains it -- but only when it is non-null, so
 * a null stays null and the nation read below dereferences it anyway. That is the shipped
 * behaviour and it is left alone.
 *
 * It takes TWO -pointer locals and a ternary, not one pointer adjusted in place: the
 * shipped frame is 8 bytes and both branches store to the same slot at the join, which an
 * `if` without an `else` does not produce. `w` is declared FIRST and `cur` second: the
 * shipped puts the `GetEventWindow` result at bp-0xa and the adjusted pointer at bp-6,
 * and Borland gives the first-declared local the higher address.
 *
 * RENAMED 2026-09-26 from europe_menu_command: the Mac build's EuropeWin::EuropeButtonClick(short): EuropeWin_Init installs it as the action of all three of the window's SButtons, and it takes the button's code.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void europe_recruit_any(void);
/* same object: 4-byte `push cs; call ` */
extern "C" void set_europe(int n);

void EuropeWin_EuropeButtonClick(int cmd)
{
    EuropeWin *w;
    EuropeWin *cur;

    cur = (EuropeWin *)GetEventWindow();
    w = (cur != 0) ? (EuropeWin *)((char *)cur - 0x26) : cur;
    set_europe(w->nation);
    switch (cmd) {
    case 0xc8:
        europe_recruit(0, 0);
        break;
    case 0xc9:
        europe_purchase();
        break;
    case 0xca:
        europe_recruit_any();
        break;
    }
}
