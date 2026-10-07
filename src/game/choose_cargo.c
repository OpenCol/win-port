/* choose_cargo: candidate for 1028:cc32 (184 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: choose_cargo
 * name-source: sibling-build
 *
 * Pops up the sixteen goods under a caller-supplied title and returns which one was picked.
 * The entries are `strings(g_goods_name_word[i])` -- the same SEG20:0xd86e table
 * four other matched files already name -- and sixteen is the number of goods.
 *
 * NAME: the Macintosh build's `choose_cargo(char*)`. One `char*` argument, and this takes
 * one pointer and nothing else. Arity and role together, the §96 argument.
 *
 * The return is the popup's event MINUS TWO, in two steps the source spells separately:
 * `r = event - 1` and then `return r - 1`. Item ids run from 1 and the caller wants a
 * zero-based goods index, so one of the two is the id-to-index shift and the other is the
 * popup's own off-by-one; the shipped `DEC AX` and `LEA AX,[DI-1]` are what keep them apart.
 *
 * `r` is initialised to 0 in its declaration even though every path overwrites it -- the
 * `XOR DI,DI` is there in the shipped prologue and does not appear without it.
 */

#include "colonization.h"

#include "goods.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern char *strings(int n);

int choose_cargo(char *title)
{
    int r = 0;
    register int i;
    Popup p(0x800);

    show_plain_dialog_1028_e43b(&p, title, 0);
    p.flags |= 1;
    for (i = 0; i < GOODS_COUNT; i++)
        Popup_item(&p, strings(g_goods_name_word[i]), i + 1);
    r = popup_run_thunk_1028_e46b(&p) - 1;
    return r - 1;
}

