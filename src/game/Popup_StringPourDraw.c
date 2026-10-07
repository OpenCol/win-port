/* Popup_StringPourDraw: candidate for 1050:1ae6 (531 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws a Popup's laid-out text. The layout is four parallel arrays of 64 entries hanging off
 * the record -- the line pointer at +0xac, its length at +0x1ac, its x at +0x22c and its y at
 * +0x2ac -- and the loop runs until the line pointer is null.
 *
 * THREE CHARACTERS ARE MARKUP, and they are the reason the drawing is done a piece at a time.
 * `{` switches to the colour at +0x96 and `}` to the colour at +0x92; `|` abandons the rest of
 * the line. Everything between markers is drawn as one string: the scan walks a second pointer
 * forward to the next marker or the end, writes a NUL over it, draws, puts the byte back and
 * carries on. The line's own end is NUL-terminated the same way at the top and restored at the
 * bottom -- which is why `|` has to be a `goto` past that restore.
 *
 * The width of `{` is measured once, and each marker consumed adds it to a running total that
 * is SUBTRACTED from the next line's x -- but only while the y does not change, and the total
 * resets when it does. That is a continuation line pulling back under the markup it swallowed.
 *
 * `{`, `}` and `|` are 0x7b, 0x7d and 0x7c and the source lists them in THAT order: Borland
 * sorts a three-case `switch` into an ascending compare chain but emits the bodies where the
 * source put them. The first two cases end in `continue`, which is why they jump to the loop
 * test rather than past the switch; the default ends in `break`, and a `break` on the last case
 * is the `EB 00` at the default label (rule 54). The body's trailing `continue` is a second
 * `EB 00`, and without it the last statement just falls into the test.
 *
 * `(int)*q` ON THE THREE MARKER TESTS. The shipped loads, `CBW`s and compares words there, and
 * folds only the `!= 0` test into `CMP BYTE [ES:BX],0`; without the cast Borland folds all four
 * and the loop is eight bytes shorter (rules 51 and 59).
 *
 * `s = q = p->line[i]` as one chained assignment: the shipped computes the address once and
 * stores it twice, where two statements recompute it (-Od folds nothing).
 *
 * The two arguments to `MSGetTextLength_1050_8391` are a `Font *` and a `char *`,
 * not a record and two scalars -- eight copies of that forwarder and `Popup_button_x_size`
 * said three, and this call site is the first that passes a NAMED string (§119). It is
 * -called with the 4-byte `PUSH CS` form and so is a stub here (rule 47).
 *
 * RENAMED 2026-09-26 from popup_draw_text: the Mac build's Popup::StringPourDraw(void): this only, drawing the lines Popup_StringPourCalculate laid out. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
typedef struct { void *win; void *dest_port; char port[0x26]; char portrait[0x20]; char tile[0x20]; void *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; char arena[14]; void *radios; void *checks; void *edit_boxes; void *buttons; void *listbox; void *regions; char *extra_labels[3]; struct { int cmd; int x; } btn[6]; } Popup;   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

extern void GRPort_String(GRPort *port, unsigned *hf, const char *s, int x, int y, int align);

/* same object: 4-byte `push cs; call ` */
extern int MSGetTextLength_1050_8391(Font *font, const char *s);

void Popup_StringPourDraw(Popup *p)
{
    int i;
    char *s;
    char *q;
    char *end;
    char qsave;
    char endsave;
    int prev;
    int markw;
    register int acc;
    register int adv;

    i = 0;
    markw = MSGetTextLength_1050_8391(p->font, "{");
    acc = 0;
    prev = p->line_y[i];
    while (p->line[i] != 0) {
        s = q = p->line[i];
        end = s + p->len[i];
        endsave = *end;
        *end = 0;
        adv = p->line_x[i];
        if (p->line_y[i] == prev)
            adv -= acc;
        else
            acc = 0;
        prev = p->line_y[i];
        while (*s != 0) {
            switch (*s) {
            case 0x7b:
                TextColor(p->hilite_color);
                s++;
                q++;
                acc += markw;
                continue;
            case 0x7d:
                TextColor(p->text_color);
                s++;
                q++;
                acc += markw;
                continue;
            case 0x7c:
                goto next;
            default:
                break;
            }
            do {
                q++;
            } while ((int)*q != 0x7b && (int)*q != 0x7d && *q != 0 && (int)*q != 0x7c);
            qsave = *q;
            *q = 0;
            GRPort_String(p->win, (unsigned *)p->font, s, p->box_x + adv, p->box_y + p->line_y[i], 5);
            adv += MSGetTextLength_1050_8391(p->font, s);
            *q = qsave;
            s = q;
            continue;
        }
        *end = endsave;
next:
        i++;
    }
}
