/* popup_run_thunk_1018_b44f: candidate for 1018:b44f (35 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `Popup_exec` with a zero `long`: the popup
 * layer's entry that `ColonyWin_ColonyClickUnits` also reaches. One of SEVEN copies, one per
 * module.
 *
 * The zero is one 32-bit `66 6A 00` push, which is what says the second argument is a
 * `long` and not two words.
 *
 * 35 bytes, not the census 31: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is a call and is `extern`.
 */

#include "colonization.h"

typedef struct Popup Popup;
extern int Popup_exec(Popup *popup, char *text);

int popup_run_thunk_1018_b44f(void *p)
{
    return Popup_exec((Popup *)p, 0);
}
