/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of GRPort_String_curfont, Sprite_GetHotSpot removed: the header declares them from the definition (W1 step 3). */
/* Popup_draw: candidate for 1050:4829 (2018 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE POPUP'S FULL REPAINT, and the reason `Popup_make_window` names it. It builds the
 * window if there is not one, paints the background and the body text, lays out the item list with its
 * sprites and captions, draws the edit fields' labels, adds the indicators, and then either closes the dialog
 * or shows every control it owns.
 *
 * THE CAPTION MARKUP is the interesting part. An item's text is drawn a RUN AT A TIME, split on three
 * markers: '{' switches to the colour byte at +0x96, '}' switches to +0x92, and '|' ends the item's
 * text altogether. Between markers, the scan walks forward to the next marker or the NUL, WRITES A NUL
 * OVER IT, draws the run, measures it to advance the pen, and PUTS THE ORIGINAL CHARACTER BACK. The
 * item's string is therefore modified in place and restored on every repaint.
 *
 * TWO LAYOUTS. With +0x7c clear the items go down the page, each sprite drawn at the running y with its
 * hot spot subtracted and its caption to the right, and afterwards the first item with a non-zero id is
 * made the selection and highlighted. With +0x7c set the first item is drawn on its own and the rest
 * flow across the page, wrapping when the accumulated width passes +0x358.
 *
 * A SHIPPED BUG in that second layout: when the row wraps, only the width accumulator is reset. The x
 * cursor keeps growing, so the second row starts where the first one ended rather than at the left
 * margin, and every row after the first is drawn off the right of the popup.
 *
 * THE SPRITE DRAW IS OVERRIDABLE. A function pointer at +0x38e, when set, replaces `Sprite_Draw`
 * and gets the sprite, the port, the item's two words and the position -- so a popup can draw its own
 * rows. All three drawing sites test it, and all three are otherwise identical.
 *
 * WHAT THE BYTES SETTLED: `col` is a register variable DECLARED INSIDE the else branch, not at function
 * scope. At function scope Borland never gives it SI, because `w->result = item->id;` assigns between two
 * pointers and needs `LES SI` as a scratch, which takes SI out of the pool for the whole function
 * and puts the frame two bytes over (rule 82). The switch's `default:` is a bare `break;` with the
 * whole scan-and-draw AFTER the switch, which is what puts rule 54's `EB 00` between the '|' arm and
 * the scan; the loop body then ends with an explicit `continue;`, another `EB 00`. And the scan's three
 * character comparisons carry `(int)` casts, without which Borland compares a byte.
 *
 * RENAMED 2026-09-26 from popup_layout: the Mac build's Popup::draw(void): this only, the popup's full repaint. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
typedef struct PopupSprite { int id; int f2; char *text; RECT rect; Sprite *sprite; struct PopupSprite *next; struct PopupSprite *prev; } PopupSprite;
#include "records.h"
typedef struct {
    void *win;                    /* +0x000 */
    char pad4[0x6e - 4];
    Font *font;                    /* +0x06e */
    char pad72[0x7a - 0x72];
    int n_edits;                         /* +0x07a */
    int n_items;                          /* +0x07c */
    int n_sprites;                       /* +0x07e */
    int n_buttons, n_extra;                      /* +0x080 */
    char pad84[0x86 - 0x84];
    int flags;                         /* +0x086 */
    int box_w, box_h;                      /* +0x088 */
    char pad8c[0x92 - 0x8c];
    unsigned char text_color;               /* +0x092 */
    char pad93[0x96 - 0x93];
    unsigned char hilite_color;               /* +0x096 */
    char pad97[0xa0 - 0x97];
    int pad_y;                           /* +0x0a0 */
    char pada2[0xa4 - 0xa2];
    int gap_y, gap_x, result;                 /* +0x0a4, +0x0a6, +0x0a8 */
    char padaa[0x330 - 0xaa];
    int org_x, org_y;                    /* +0x330 */
    char pad334[0x33c - 0x334];
    int box_x, box_y;                        /* +0x33c */
    int framed;                          /* +0x340 */
    char pad342[0x34a - 0x342];
    int sprite_x, sprite_y, edit_x, edit_y;        /* +0x34a */
    char pad352[0x354 - 0x352];
    int button_y;                          /* +0x354 */
    char pad356[0x358 - 0x356];
    int width;                          /* +0x358 */
    char pad35a[0x37a - 0x35a];
    PopupSprite *sprites;                   /* +0x37a */
    PopupSprite *cur_sprite;                 /* +0x37e */
    char pad382[0x386 - 0x382];
    PopupEdit *edit_items;                     /* +0x386 */
    char pad38a[0x38e - 0x38a];
    long draw_sprite;
    char pad392[0x3a0 - 0x392];        /* +0x38e */
    char *radios;                    /* +0x3a0 */
    SCheckBox *checks;                    /* +0x3a4 */
    SEditBox *edit_boxes;                    /* +0x3a8 */
    SButton *buttons;                    /* +0x3ac */
    char pad3b0[0x3b4 - 0x3b0];
    SRegion *regions;                    /* +0x3b4 */
} Popup;

extern int g_popup_tribe;                 /* DGROUP:0x2518, declared */

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void SRadioSet_Show(SRadioSet *p);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int Popup_compute_size(Popup *w);
extern int Popup_make_window(Popup *popup);
extern void Popup_make_controls(Popup *w);
extern void Popup_fill_area(Popup *popup, int a, int b, int c, int d);
extern void Popup_do_colors(Popup *popup, int a, int b);
extern void Popup_StringPourDraw(Popup *w);

extern int MSGetLineHeight_1050_834b(Font *font);
extern int MSGetTextLength_1050_8391(Font *font, const char *s);
extern int sprite_width_1050_83f9(Sprite *sprite);
extern int Popup_edit_y_size(Popup *popup);
extern void DrawSpriteSquare(void);
extern void Popup_DrawDude(Popup *w);
extern void show_window_1050_8bc8(void *port);

extern void MSShowControl_1050_8459(DlgControl *control);

void Popup_draw(Popup *w)
{
    register int i;
    int made;
    int x;
    int y;
    int tx;
    int ty;
    PopupEdit *edit;
    PopupSprite *item;
    int hx;
    int hy;
    int px;
    int py;
    char *p;
    char *q;
    char saved;
    RECT rc;

    if (Popup_compute_size(w) != 0)
        goto out;
    made = Popup_make_window(w);
    Popup_make_controls(w);
    Popup_fill_area(w, w->org_x, w->org_y, w->box_w, w->box_h);
    Popup_do_colors(w, 0, 0);
    Popup_StringPourDraw(w);
    if (w->n_sprites != 0) {
        if (w->n_items == 0) {
            x = w->sprite_x;
            y = w->sprite_y;
            item = w->sprites;
            while (item != 0) {
                Sprite_GetHotSpot(item->sprite, &hx, &hy);
                if (w->draw_sprite != 0)
                    (*(void (*)(void *, void *, int, int, int, int))w->draw_sprite)(item->sprite, w->win, item->id, item->f2, x, y);
                else {
                    Sprite_Draw(&item->rect, item->sprite, w->win,
                                w->box_x + x - hx + 6, w->box_y + y - hy + 3);
                    InflateRect(&item->rect, 2, 2);
                }
                if (item->text != 0) {
                    tx = w->gap_x + x + 0x20;
                    ty = ((rect_height_1050_83da(item->sprite)
                           - MSGetLineHeight_1050_834b(w->font)) >> 1) + (-hy + y);
                    p = q = item->text;
                    px = tx + 6;
                    py = ty + 3;
                    while (*p != 0) {
                        switch (*p) {
                        case 0x7b:
                            TextColor(w->hilite_color);
                            p++;
                            q++;
                            continue;
                        case 0x7d:
                            TextColor(w->text_color);
                            p++;
                            q++;
                            continue;
                        case 0x7c:
                            goto text_done;
                        default:
                            break;
                        }
                        do
                            q++;
                        while ((int)*q != 0x7b && (int)*q != 0x7d && *q != 0
                               && (int)*q != 0x7c);
                        saved = *q;
                        *q = 0;
                        GRPort_String(w->win, (unsigned *)w->font, p, w->box_x + px,
                                             w->box_y + py, 5);
                        px += MSGetTextLength_1050_8391(w->font, p);
                        *q = saved;
                        p = q;
                        continue;
                    }
                }
text_done:
                y += rect_height_1050_83da(item->sprite);
                y += w->gap_y;
                item = item->next;
            }
            item = w->sprites;
            while (item != 0) {
                if (item->id != 0) {
                    w->result = item->id;
                    w->cur_sprite = item;
                    DrawSpriteSquare();
                    break;
                }
                item = item->next;
            }
        } else {
            register int col;

            x = w->sprite_x;
            y = w->sprite_y;
            item = w->sprites;
            Sprite_GetHotSpot(item->sprite, &hx, &hy);
            if (w->draw_sprite != 0)
                (*(void (*)(void *, void *, int, int, int, int))w->draw_sprite)(item->sprite, w->win, item->id, item->f2, x, y);
            else {
                Sprite_Draw(&item->rect, item->sprite, w->win,
                            w->box_x + x - hx + 6, w->box_y + y - hy + 3);
                InflateRect(&item->rect, 2, 2);
            }
            col = 0;
            x = w->sprite_x + 0x20;
            y = w->button_y - w->pad_y - 0x20;
            if (w->sprites->next != 0) {
                item = w->sprites->next;
                while (item != 0) {
                    Sprite_GetHotSpot(item->sprite, &hx, &hy);
                    if (w->draw_sprite != 0)
                        (*(void (*)(void *, void *, int, int, int, int))w->draw_sprite)(item->sprite, w->win, item->id, item->f2, x, y);
                    else {
                        Sprite_Draw(&rc, item->sprite, w->win,
                                    w->box_x + x - hx + 6, w->box_y + y - hy + 3);
                        InflateRect(&item->rect, 2, 2);
                    }
                    x += sprite_width_1050_83f9(item->sprite) + w->gap_x;
                    col += sprite_width_1050_83f9(item->sprite) + w->gap_x;
                    if (w->width < col) {
                        col = 0;
                        y += w->gap_y + 0x20;
                    }
                    item = item->next;
                }
                y += 0x20;
                y += w->gap_y;
            }
        }
    }
    if (w->n_edits != 0) {
        edit = w->edit_items;
        i = 0;
        while (edit != 0) {
            GRPort_String_curfont(w->win, edit->label, w->box_x + w->edit_x + 6,
                                Popup_edit_y_size(w) * i + (w->box_y + w->edit_y), 5);
            i++;
            edit = edit->next;
        }
    }
    if (w->framed != 0 && g_popup_tribe != 5)
        Popup_DrawDude(w);
    if (made != 0) {
        show_window_1050_8bc8(w->win);
        MSDoAllTasks_1050_8be7();
    } else {
        copy_all_to_screen_1050_843b(w->win);
        if (w->n_items != 0) {
            if (w->flags & 4) {
                for (i = 0; w->n_items > i; i++)
                    MSShowControl_1050_8459((DlgControl *)&w->checks[i]);
            } else
                SRadioSet_Show((SRadioSet *)w->radios);
        }
        if (w->n_edits != 0) {
            for (i = 0; w->n_edits > i; i++)
                MSShowControl_1050_8459((DlgControl *)&w->edit_boxes[i]);
        }
        if (w->n_buttons != 0) {
            for (i = 0; w->n_buttons + w->n_extra > i; i++)
                MSShowControl_1050_8459((DlgControl *)&w->buttons[i]);
        }
        if (w->n_sprites > 0 && w->n_items == 0) {
            for (i = 0; w->n_sprites > i; i++)
                MSShowControl_1050_8459((DlgControl *)&w->regions[i]);
        }
        MSDoAllTasks_1050_8be7();
    }
    w->flags |= 0x20;
out:
    ;
}
