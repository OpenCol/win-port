/* dispatch_click: candidate for 1050:69b6 (96 bytes).  Verdict: RELOCATABLE.
 * symbol: dispatch_click
 *
 * Takes the current window, steps back to the object that owns it, and passes the click
 * point and icon sprite 0x65 to this module's handler, which answers through two
 * out-parameters. The sprite was declared as its own global `g_label` at SEG20:0x6ee6,
 * which is g_icon_sprites (0x6246) + 0x65 * 0x20: the drag icon, not a text label.
 *
 * RENAMED 2026-09-27 from dispatch_click_1050_69b6: its family's other copy took the Mac build's own name in this batch (1030:091b SetDifficulty_LeftMouseClick / 1010:b882 ColonyWin_AttemptShipToShip), so this is now the family's only member and drops its address suffix (check_naming: a singleton carries no suffix).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

void dispatch_click(int x, int y)
{
    char *c;
    int out_x;
    int out_y;
    char *w;

    w = (char *)GetEventWindow();
    c = w != 0 ? w - 0x26 : w;
    drag_sprite(c, &g_icon_sprites[0x65], x, y, &out_x, &out_y);
}
