/* Popup_dtor: candidate for 1050:07ac (173 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Popup::~Popup()
 *
 * The destructor of Popup: Popup_close, the memblock at +0x392 freed, Popup_clear, and
 * win_enable_1050_82e1 on the pointer at DGROUP:0x24fc; the Sprite and GRPort members are the
 * compiler's.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GRPort, Sprite, EventWin, DialogWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a

#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void Popup_clear(Popup *popup);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void Popup_close(Popup *popup);
extern void heap_destroy(MemBlock *block);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void win_enable_1050_82e1(void *w);
   /* EventWin : Screen, EventTable, flattened for C: 0x8c bytes */
   /* DialogWin : EventWin, flattened for C */

Popup::~Popup()
{
    Popup_close(this);
    heap_destroy(&arena);
    Popup_clear(this);
    win_enable_1050_82e1((DialogWin *)g_popup_owner);
}
