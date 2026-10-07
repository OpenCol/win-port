/* MagicWin_remap_using_palette: candidate for 1068:ab31 (47 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MagicWin_remap_using_palette
 * name-source: descriptive
 *
 * Points the MagicWin at a target palette and remaps: stores the pointer into the slot at
 * +0xea that `MagicWin_remap_to_palette` reads, then calls it for `count` entries from
 * `first`. It sits 47 bytes above that function and is the only way the slot is set.
 *
 * The store is a single 32-bit `MOV [es:bx+0xea],EAX`, which is what `-3` gives for a 
 * pointer assignment.
 *
 * RENAMED 2026-09-27 from sprite_remap_using_palette: the receiver is a MagicWin, not a Sprite: port at +0xc4, palette at +0xea, and every caller sits in MagicWin's module 1068:a698..b188 (521cb981e). The Mac build has no such method; the name stays descriptive.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
struct logpal;

/* same object: 4-byte `push cs; call ` -- MagicWin_remap_to_palette is 1068:ab60 */
extern void MagicWin_remap_to_palette(MagicWin *w, short first, short count);

void MagicWin_remap_using_palette(MagicWin *w, short first, short count,
                                    struct logpal *pal)
{
    w->pal = pal;
    MagicWin_remap_to_palette(w, first, count);
}
