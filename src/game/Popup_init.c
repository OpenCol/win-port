/* PORT: this file's own declarations of Popup_set_heap removed: the header declares it from the definition (W1 step 3). */
/* Popup_init: candidate for 1050:0859 (269 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Points a Popup at a new pair of values and a new rectangle. Bit 5 of the kind word means one
 * is already up, and only then is the old one taken down first -- three calls, the middle one
 * carrying the word at +0x39c.
 *
 * A NULL rectangle is not an error: the width falls back to 0x1b8 and the height and position
 * are left as they were. With a rectangle, the top-left corner goes to +0x330/+0x332 and the width and
 * height through the two setters.
 *
 * The kind word is ASSIGNED from the parameter and then has bit 4 ORed in when the first long
 * is non-zero, so the caller cannot pass that bit itself -- it is derived.
 *
 * THE VALUE AT DGROUP:0x24f0 IS A FAR POINTER. The shipped pushes 0x24f2 and then 0x24f0, the
 * segment first, which is a pointer variable; a `long` there is one `PUSH DWORD` and three
 * bytes shorter.
 *
 * The five calls without a NOP (the three Popup calls and the two rect helpers) are stubs; `Popup_set_width`, `Popup_set_height` and
 * `Popup_set_font` come through the five-byte form and are extern (rule 47).
 *
 * RENAMED 2026-09-27 from popup_setup: CONST PROPOSAL. Mac Popup::init(GameWin*, GRPort*, Rect*, int), CODE 19 0x1b12, 218 bytes. 0859 (Popup *popup, long win, long port, RECT *r, int kind): this plus two pointers, a rect pointer and an int. Callers: Mac CheatMenuBlow, orders_activate, Popup::start; Windows popup_setup_plain_1008_ce18, popup_setup_plain_1048_e387, text_directive_parser (= Popup::start, below). Callees: Popup close, set_heap and clear on both (Windows also calls set_font/set_width/set_height, which the Mac inlines). Layout: Mac Popup dtor, init, string, help_me, item_x_size; Windows Popup_dtor, 0859, Popup_string, Popup_help_me, Popup_item_x_size. Size 218 against 269. Constants: overlap 1.0. popup_setup is a prefix of the two popup_setup_plain_* thunks, so rename by exact word. Update the prose in tools/check_split_args.py:53.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

/* same object: 4-byte `push cs; call ` */
extern void Popup_close(Popup *popup);
extern void Popup_clear(Popup *popup);

/* 5-byte NOP form, so extern (rule 47) */
extern void Popup_set_width(Popup *popup, int width);
extern void Popup_set_height(Popup *popup, int height);
extern void Popup_set_font(Popup *popup, Font *new_font);

void Popup_init(Popup *popup, GameWin *win, GRPort *port, RECT *r, int kind)
{
    if (popup->flags & 0x20) {
        Popup_close(popup);
        Popup_set_heap(popup, popup->arena.size);
        Popup_clear(popup);
    }
    popup->win = win;
    popup->dest_port = port;
    popup->result = 0;
    if (r != 0) {
        popup->org_x = r->left;
        popup->org_y = r->top;
        Popup_set_width(popup, rect_width_1050_830c(r));
        Popup_set_height(popup, rect_height_1050_832b(r));
    } else {
        Popup_set_width(popup, 0x1b8);
    }
    popup->flags = kind;
    Popup_set_font(popup, g_popup_font);
    if (win != 0)
        popup->flags |= 0x10;
}
