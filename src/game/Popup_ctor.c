/* Popup_ctor: candidate for 1050:03c2 (179 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Popup::Popup(int)
 *
 * The constructor of Popup (0x3dc bytes; a GRPort at +8 and two Sprites at +0x2e/+0x4e):
 * the reset routine Popup_clear twice around a 9-byte-per-entry memblock of n entries at
 * +0x392, then the port initialised 40x40. The reset routine is defined in this file
 * because the shipped calls to it are the compiler's own `push cs / call `: it zeroes
 * the popup's bookkeeping, sets the -1 sentinels, takes the four default COLOUR bytes (text, highlight, and two more -- not margins) from
 * DGROUP:0x24f8..0x24fb and the fixed spacing (2, 2, 5, 4) and sets the popup's column count at +0x84 to 1.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern void Popup_set_font(Popup *popup, Font *owner);
extern int heap_create(MemBlock *heap, int kind, unsigned size);

extern "C" extern "C" void Popup_clear(Popup *popup);

Popup::Popup(int n)
{
    Popup_clear(this);
    heap_create(&arena, 9, n);
    Popup_clear(this);
    GRPort_Init_wh(&port, 0x28, 0x28);
}
