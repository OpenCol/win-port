/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 69 of the imported file. */
/* route_get: candidate for 1028:c83a (441 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: route_get
 * name-source: sibling-build
 *
 * The trade-route picker. A stack Popup lists every route (numbered, "N. " plus the name
 * the route record starts with) that fits `kind` -- 0 = all, 1 = land routes, 2 = sea
 * routes, against the route's `sea` byte at +0x20 -- preselecting `route`. No routes at
 * all gives TRADENONE; routes that all fail the filter give TRADENONE2 with the land/sea
 * word spliced in. Returns the chosen route index, or -1.
 *
 * THE SAME COMPARISON IS WRITTEN TWO DIFFERENT WAYS AND COMPILES TWO DIFFERENT WAYS, and
 * this was the whole difficulty -- it sat at 0x87 of 441 as a partial until both were
 * spelled right.
 *
 * The filter in the loop is a BYTE compare against memory with the pointer spilled:
 *
 *     les bx,[0x4c18] / push es / push bx / <ternary into AX> / pop bx / pop es
 *     cmp [es:bx+0x20],al
 *
 * That is `g_cur_route_rec->sea == (unsigned char)(kind == 2)`. The cast is what keeps the
 * left side unpromoted, so the compare stays byte-wide and Borland has to hold the pointer
 * across the branchy right operand instead of loading the value first.
 *
 * The message at the end is the WORD compare, value-first:
 *
 *     les bx,[0x4c18] / mov al,[es:bx+0x20] / mov ah,0 / push ax / <...> / pop dx
 *     cmp dx,ax
 *
 * That is the same expression with no cast -- `g_cur_route_rec->sea == (kind == 2)` -- so
 * the char promotes to int and the promoted value is what gets spilled.
 *
 * And in BOTH, `(kind == 2)` and `(kind == 2 ? 1 : 0)` are not interchangeable even though
 * they generate the same five instructions. With the ternary, Borland evaluates the RIGHT
 * operand of `==` first and the whole `les` moves after it; with the bare relational it
 * evaluates left to right. That is rule 42.
 *
 * RENAMED 2026-09-26 from no_trade_routes_defined: the Mac build's route_get(short, short, char*), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller route_delete; caller route_edit; callee popup_say; callee say_number), the runner-up gets 2, and the arity agrees (3).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void set_dest(int r);

extern void period_string(char *buf);
extern void Popup_item(Popup *popup, char *s, int id);
extern void Popup_set_active(Popup *popup, int id);
extern void popup_say(int slot, int word);
extern "C" void show_text_dialog_a(char *msg, int kind);

extern "C" int route_get(int kind, int route, char *title)
{
    int sel = -1;
    int n;
    int count = 0;
    Popup popup(0x800);
    char buf[0x50];
    int i;

    if (g_game.trade_route_count < 1) {
        show_text_dialog_1028_e48e((void*)("TRADENONE"));
        goto done;
    }
    show_plain_dialog_1028_e43b(&popup, title, 0);
    popup.flags |= 1;
    for (i = 0; g_game.trade_route_count > i; i++) {
        set_dest(i);
        if (kind == 0 || g_cur_route_rec->sea == (unsigned char)(kind == 2)) {
            buf[0] = 0;
            say_number(buf, i + 1);
            period_string(buf);
            STRCAT(buf, (char *)g_cur_route_rec);
            Popup_item(&popup, buf, i + 1);
            count++;
            if (i == route)
                Popup_set_active(&popup, i + 1);
        }
    }
    if (count == 0) {
        popup_say(0, g_route_label[(g_cur_route_rec->sea == (kind == 2)) ? 3 : 4]);
        show_text_dialog_a("TRADENONE2", 0);
        goto done;
    }
    n = popup_run_thunk_1028_e46b(&popup) - 1;
    if (n > 0)
        sel = n - 1;
done:
    return sel;
}
