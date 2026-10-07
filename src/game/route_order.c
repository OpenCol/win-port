/* PORT: this file's own declarations of sprite_width_1058_c203 removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of set_order removed: the header declares it from the definition (W1 step 3). */
/* route_order: candidate for 1058:8635 (424 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A click on one side (0 or 1) of the trade-route cargo strip at x: the goods icons (32-byte
 * sprite records at SEG20:0x6526) are laid out from x = 225 or 425 with their widths; a hit
 * removes that good from the side's list (the rest shift down); a click past the icons, with
 * fewer than six goods, asks for a good to add (CARGOUNLOAD / CARGOLOAD) and appends it.
 * Nothing happens while the route record (+0x21 of the pointer at DGROUP:0x4c18) has no
 * more stops than DGROUP:0x4c22.
 *
 * THE THIRD PARAMETER IS REAL AND THE BODY IGNORES IT. This function reads only `side` and
 * `x`, so its own bytes are those of a two-parameter function -- §119's blind spot. Its one
 * caller, `route_box_parse`, pushes three words and clears six, and the third is
 * the `y` that caller already used to pick the route. Corrected 2026-09-13.
 *
 * RENAMED 2026-09-26 from select_cargo_to_transfer: the Mac build's route_order(short, short, short), bound by call-graph agreement: 2 of its Mac call edges already bound land on this function (callee choose_cargo; callee dest_name), the runner-up gets 0; the arity agrees (3), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

#include "icons.h"

extern int get_num_orders(int side);
extern int get_order(int slot);
extern void set_num_orders(int side, int count);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern char *dest_name(int stop);

void route_order(int side, int x, int y)
{
    int px;
    int j;
    int base;
    int start;
    int i;
    int good;
    int n;
    int sel = -1;

    if (g_cur_route_rec->count <= g_route_stop)
        goto done;
    base = side ? 6 : 0;
    start = side ? 0x1a9 : 0xe1;
    n = get_num_orders(side);
    px = start;
    for (i = 0; i < n && sel < 0; i++) {
        good = get_order(base + i);
        px += sprite_width_1058_c203(&g_icon_sprites[ICON_GOODS + good]) / 2 + 6;
        px += sprite_width_1058_c203(&g_icon_sprites[ICON_GOODS + good]) / 2 + 6;
        if (x < px)
            sel = i;
    }
    if (sel < 0 && n >= 6)
        goto done;
    if (sel >= 0) {
        for (j = sel; n - 1 > j; j++)
            set_order(base + j, get_order(base + j + 1));
        set_num_orders(side, n - 1);
    } else {
        set_name_slot_1058_c2a5(0, dest_name(g_route_stop));
        if (side != 0)
            good = choose_cargo("CARGOLOAD");
        else
            good = choose_cargo("CARGOUNLOAD");
        if (good < 0)
            goto done;
        set_order(base + n, good);
        set_num_orders(side, n + 1);
    }
done:
    return;
}
