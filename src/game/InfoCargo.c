/* PORT: this file's own declarations of sprite_width_1058_c203 removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of port_width_1058_bcfb removed: the header declares it from the definition (W1 step 3). */
/* InfoCargo: candidate for 1058:8dd6 (1127 bytes).  Verdict: RELOCATABLE.
 * name-source: the Macintosh sibling build, matched by call-site arity
 *
 * DRAWS A UNIT'S CARGO into the info window, at (x, y). Nothing in the Windows image calls it.
 *
 * THE ORDER: the slots are sorted by value -- the owner's goods value (SEG30:0x10, 16 per nation) times
 * the amount, with muskets and tools counted as 0 and horses as 1 so they sort last. The sort keeps the
 * slot numbers in a parallel array.
 *
 * THE FIT: the row is measured at scale 8/8 and, while it would run past the window's width less 20, the
 * scale is dropped by one eighth and it is measured again. The loop also stops when a pass comes out the
 * same width as the one before, so a row that cannot be shrunk any further does not spin.
 *
 * EACH SLOT is an icon 0x27 or, for 100 or more, 0x17, plus the goods index. It is stepped over by half
 * its scaled width before and after, plus 6 -- so the icons are spaced by their own widths. CIGARS AND
 * MUSKETS (10 and 15) are drawn at 1:1 whatever the scale, with the scale switched back around the draw.
 *
 * THE FIRST FLOATING-POINT FUNCTION in the corpus, and it fixes the shape of Borland's x87 code: the
 * scale factor is `zoom / 8.0f`: DGROUP:0x2876 is that float literal (an `FDIV` with a 4-byte
 * operand), the unscaled arm is `* 1.0` (`FLD1`), each `int` goes into one reused stack temp and comes
 * back with `FILD`, and `(int)` of the result is `F_FTOL@`. That temp sits above the arrays, which is
 * why they are declared in an inner block (rule 77).
 *
 * The shrink test is `if (...) zoom--; else goto done;`, which is what puts the two-byte trampoline
 * between the decrement and the next test.
 */

#include "colonization.h"

#include "w16types.h"
#include "goods.h"
#include "units.h"
#include "classes.h"
#include "records.h"

extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);
extern void sort_16x16(int n, int *payload, int *key);
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

/* same object: 4-byte `push cs; call ` (rule 47) */

void InfoCargo(GRPort *port, int u, int x, int y)
{
    int i;
    int amount;
    int icon;
    int n;
    int value;
    int zoom;
    int slot;
    int prev;
    register int type;
    register int w;

    n = g_units[u].cargo_count;
    {
    RECT r1;
    RECT r2;
    int mx;
    int idx[6];
    int key[6];
    for (i = 0; i < n; i++) {
        idx[i] = (unsigned char)i;
        type = get_cargo(u, i);
        amount = get_cargo_amount(u, i);
        value = g_goods_value[(unsigned char)(g_units[u].flags & UNIT_NATION_MASK)][type] << 4;
        if (type == GOODS_MUSKETS)
            value = 0;
        if (type == GOODS_TOOLS)
            value = 0;
        if (type == GOODS_HORSES)
            value = 1;
        key[i] = value * amount;
    }
    sort_16x16(n, idx, key);
    zoom = 8;
    prev = -1;
again:
    mx = x + 0xe;
    for (i = 0; i < n; i++) {
        slot = idx[i];
        type = get_cargo(u, slot);
        amount = get_cargo_amount(u, slot);
        if (amount <= 0)
            continue;
        icon = amount >= 100 ? 0x17 : 0x27;
        if (type != GOODS_CIGARS && type != GOODS_MUSKETS)
            w = (int)(sprite_width_1058_c203(&g_icon_sprites[icon + type]) * (zoom / 8.0f));
        else
            w = (int)(sprite_width_1058_c203(&g_icon_sprites[icon + type]) * 1.0);
        w = w / 2;
        mx += w;
        if (type != GOODS_CIGARS && type != GOODS_MUSKETS)
            w = (int)(sprite_width_1058_c203(&g_icon_sprites[icon + type]) * (zoom / 8.0f));
        else
            w = (int)(sprite_width_1058_c203(&g_icon_sprites[icon + type]) * 1.0);
        w = w / 2;
        mx += w + 6;
    }
    if (x + 0xe == mx)
        goto done;
    if (port_width_1058_bcfb(port) - 0x14 < mx && mx > 1)
        zoom--;
    else
        goto done;
    if (prev == mx)
        goto done;
    prev = mx;
    goto again;
done:
    SetGlobalSpriteScale(zoom, 8);
    x += 0xe;
    for (i = 0; i < n; i++) {
        slot = idx[i];
        type = get_cargo(u, slot);
        amount = get_cargo_amount(u, slot);
        if (amount <= 0)
            continue;
        icon = amount >= 100 ? 0x17 : 0x27;
        if (type != GOODS_CIGARS && type != GOODS_MUSKETS)
            w = (int)(sprite_width_1058_c203(&g_icon_sprites[icon + type]) * (zoom / 8.0f));
        else
            w = (int)(sprite_width_1058_c203(&g_icon_sprites[icon + type]) * 1.0);
        w = w / 2;
        x += w;
        if (type != GOODS_CIGARS && type != GOODS_MUSKETS) {
            Sprite_Draw(&r1, &g_icon_sprites[icon + type], port, x, y);
            w = (int)(sprite_width_1058_c203(&g_icon_sprites[icon + type]) * (zoom / 8.0f));
        } else {
            SetGlobalSpriteScale(1, 1);
            Sprite_Draw(&r2, &g_icon_sprites[icon + type], port, x, y);
            w = (int)(sprite_width_1058_c203(&g_icon_sprites[icon + type]) * 1.0);
            SetGlobalSpriteScale(zoom, 8);
        }
        w = w / 2;
        x += w + 6;
    }
    SetGlobalSpriteScale(1, 1);
    }
}
