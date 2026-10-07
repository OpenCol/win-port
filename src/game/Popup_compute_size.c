/* PORT: this file's own declarations of CTable_Init, Sprite_Init removed: the header declares them from the definition (W1 step 3). */
/* Popup_compute_size: candidate for 1050:1d16 (3868 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Popup_compute_size
 * name-source: sibling-build
 *
 * HOW BIG A POPUP HAS TO BE, and where every piece of it goes. It runs before
 * `Popup_make_controls` creates the controls: this function only measures and places,
 * writing the size into +0x88/+0x8a and a dozen sub-origins into the block at +0x32c. It returns 1
 * when the popup has nothing in it and 0 when it was laid out.
 *
 * FIRST it settles a conflict: a popup asked for BOTH edit fields and menu items keeps the items and
 * drops the edits. Then the interior starts at one padding all round -- the body, region, text and
 * edit origins are all set to (+0xa2, +0xa0) and the box to twice that -- and the three widths at
 * +0x356, +0x358 and +0x35e are levelled to the largest of them, which is what makes a popup's
 * columns line up. With no items, regions or edits at all it stops right there.
 *
 * THE REGION LIST is measured in one of three ways. When the popup has NO menu items, or when any
 * region carries something at +4, the regions are STACKED: each one's height plus a gap. When there
 * is exactly ONE region it takes no height of its own. Otherwise the regions FLOW into rows:
 * widths are added up until the running total passes +0x358, and each overflow starts a new row
 * (+0x382 counts the wraps, +0x384 is the height, 0x20 plus a gap per row). All three arms shift the three left origins right by
 * the region-column width at +0x35a.
 *
 * THE TEXT, the MENU and the EDIT FIELDS each contribute a height. The text is laid out for real by
 * `Popup_StringPourCalculate` (with no text the first line pointer is nulled so nothing is drawn);
 * the menu is `row_h` times `(cols + count - 1) / max(1, cols)` rows, or the fixed list height at
 * +0x8e when the list-box bit 0x1000 is set; the edits are one row height each. The three are added
 * and a gap is inserted when text sits above either of the others. That column's height and the
 * region height are then maxed into the box.
 *
 * THE BUTTON ROW is built next. Bit 0x40 adds one button, bit 1 adds another, and there is always a
 * last one, so a popup has one to three; the extra labels at +0x3b8 (Popup_button) add up to three more. Every
 * label is measured and the widest wins, so all the buttons are the same size, and they are then
 * placed RIGHT TO LEFT from the box's right edge -- the popup's own buttons first, the extras after
 * them. A popup with exactly one button and no menu and no edits gets it centred instead.
 *
 * THE POPUP IS THEN CENTRED on the desktop, but only in the directions where the desktop is no
 * larger than 640x480; on a bigger screen the caller's own position stands. Everything inside is
 * shifted by the popup's origin, buttons included.
 *
 * FINALLY THE FRAME. Three globals select it, tried in order: DGROUP 0x251a (a portrait frame, whose
 * overhang comes from the word table at SEG31:0xc), then 0x2518 (the King's frame or a native tribe's;
 * case 8 is the King's fixed art, tribes 0..7 pick art by the tribe's anger at the player through
 * `pissed` and `piss_factor`, with cases 5 and 4 overriding the text inset),
 * then 0x251c (another portrait set, SEG31:0x38). With none of them set the frame is just the popup.
 * Each frame is centred on the desktop in its own right, so the artwork -- not the popup -- is what
 * the player sees centred.
 *
 * WHAT THE BYTES SETTLED: the early exit is `goto done`, not `return ret` -- with a `GRPort` local
 * every `return` would inline its destructor call and only a jump reaches the shared one. The
 * region/item test is written the negative way round, `if (p->sprites == 0 || p->n_items == 0) ... else`,
 * because the shipped jumps forward INTO the else arm. Every `max_ptr` temporary uses rule 61's
 * comma form, and in the two frame branches the argument holding the sprite call is the FIRST one,
 * so it is the plain `p->box_h + 0x14` that gets pushed first. And `(int)stuck == 1` needs the cast: the
 * flag is a `char` and without it the comparison folds into one byte.
 *
 * `g_frame_top_text` AND `g_frame_top_sel` WERE `g_seg31_0c` AND `g_seg31_38`, named
 * 2026-09-25 from the two arms here, which are the only sites either has and are structurally
 * identical. Each arm picks a framed style, builds its two sprites and its palette from one
 * base -- `g_popup_dude + 0xd2`/`+ 0x69` in the first, `g_popup_nation + 0xdc`/`+ 0x64` in the second
 * -- and then applies its table the same way twice:
 *
 *     p->win_h = -TABLE[style] + p->box_h + 0x14;      the frame's height
 *     p->box_y = -TABLE[style] +        0xa;           the content's top inside it
 *
 * Subtracting one value from BOTH the height and the top origin is a top inset: the frame grows
 * upward by that much and the content sits that much lower inside it, which is what a decorated
 * top sprite that overhangs its content needs. The entries are therefore negative.
 *
 * NAMED FOR THE ROLE AND THE INDEX, which is all two sites establish. What the style numbers in
 * `g_popup_dude` (DGROUP:0x251a) and `g_popup_nation` (0x251c) actually SELECT is not settled here, so
 * if either of those is renamed later these two should follow it.
 *
 * RENAMED 2026-09-26 from popup_measure: the Mac build's Popup::compute_size(void): this only, working out the popup's size and the origins of its parts before the controls are made. It sits in the Popup module (1050:0000..50c4) and the width matches.
 *
 * THE TWO PORTRAIT SELECTORS were g_sel_b and g_sel_c until 2026-09-27: g_popup_nation
 * (DGROUP:0x251c) is the European nation whose leader is shown (text_dialog_sel_b0 -- the
 * Mac popf -- is passed `owner`/`their_nation`), sprite g_popup_nation + 0xdc; g_popup_tribe
 * (0x2518) is a tribe 0..7, drawn by its anger with the player (pissed(tribe, player)), or 8
 * for the King's fixed frame.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own CTable, GRPort, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* 0x41e bytes: the palette, its handle and the cycling state */


#include "records.h"
   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
typedef struct PopupSprite { int id; int f2; char *text; RECT rect; Sprite *sprite; struct PopupSprite *next; struct PopupSprite *prev; } PopupSprite;
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the arena (a MemBlock) as its 14 bytes */

extern int g_frame_top_text[];                 /* SEG31:0xc */
extern int g_frame_top_sel[];                 /* SEG31:0x38 */

extern GameState g_game;   /* SEG20:0x7782 */
extern int g_popup_tribe;                       /* DGROUP:0x2518, declared */

extern int g_popup_nation;                       /* DGROUP:0x251c, declared */


extern int pissed(int tribe, int i);
extern int piss_factor(int tension);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int sprite_width_1050_83f9(Sprite *sprite);
extern "C" int Popup_button_x_size(Popup *ctl, char *s);
extern "C" int Popup_button_y_size(Popup *popup);
extern "C" int Popup_edit_y_size(Popup *popup);
extern "C" int Popup_StringPourCalculate(Popup *p);

int Popup_compute_size(Popup *p)
{
    int ret = 1;
    int region_w;
    int text_h = 0;
    int radio_h = 0;
    int edits_h = 0;
    i16 right_h;   /* PORT: i16, max_ptr_1050_97d1 takes its address */
    i16 regions_h;   /* PORT: i16, max_ptr_1050_97d1 takes its address */
    int anger;
    int rows;
    RECT desk;
    PopupSprite *r;
    char stuck;
    i16 one;   /* PORT: i16, max_ptr_1050_97d1 takes its address */
    i16 label_w;   /* PORT: i16, max_ptr_1050_97d1 takes its address */
    i16 extra_w;   /* PORT: i16, max_ptr_1050_97d1 takes its address */
    i16 popup_h;   /* PORT: i16, max_ptr_1050_97d1 takes its address */
    i16 top_h;   /* PORT: i16, max_ptr_1050_97d1 takes its address */
    i16 popup_h2;   /* PORT: i16, max_ptr_1050_97d1 takes its address */
    i16 top_h2;   /* PORT: i16, max_ptr_1050_97d1 takes its address */
    int run_w;
    register int i;
    register int bx;
    GRPort port;

    stuck = 0;
    if (p->n_edits != 0 && p->n_items != 0) {
        p->n_edits = 0;
        p->edit_items = 0;
    }
    p->pos_x = p->req_x;
    p->pos_y = p->req_y;
    p->box_w = p->pad_x * 2;
    p->box_h = p->pad_y * 2;
    p->text_x = p->pad_x;
    p->text_y = p->pad_y;
    p->item_x = p->pad_x;
    p->item_y = p->pad_y;
    p->edit_x = p->pad_x;
    p->edit_y = p->pad_y;
    p->sprite_x = p->pad_x;
    p->sprite_y = p->pad_y;
    p->width = *max_ptr_1050_97d1(max_ptr_1050_97d1(&p->width, &p->item_w), &p->edit_w);
    p->item_w = p->edit_w = p->width;
    if (p->n_items + p->n_lines + p->n_sprites + p->n_edits == 0)
        goto done;
    regions_h = 0;
    if (p->sprites != 0) {
        for (r = p->sprites; r != 0; r = r->next) {
            if (r->text != 0) {
                stuck = 1;
                break;
            }
        }
    }
    if (p->sprites != 0 && (p->n_items == 0 || (int)stuck == 1)) {
        region_w = p->sprite_w;
        p->text_x += region_w;
        p->item_x += region_w;
        p->edit_x += region_w;
        for (r = p->sprites; r != 0; r = r->next) {
            regions_h += rect_height_1050_83da(r->sprite);
            if (r->next != 0)
                regions_h += p->gap_y;
        }
    } else {
        if (p->sprites != 0 && p->sprites->next == 0) {
            region_w = p->sprite_w;
            p->text_x += region_w;
            p->item_x += region_w;
            p->edit_x += region_w;
        } else if (p->sprites != 0) {
            region_w = p->sprite_w;
            p->text_x += region_w;
            p->item_x += region_w;
            p->edit_x += region_w;
            p->sprite_wraps = 0;
            run_w = 0x20;
            p->sprites_h = 0x20;
            r = p->sprites->next;
            while (r != 0) {
                run_w += sprite_width_1050_83f9(r->sprite) + p->gap_x;
                if (p->width < run_w) {
                    p->sprite_wraps++;
                    p->sprites_h += p->gap_y + 0x20;
                    run_w = 0x20;
                }
                r = r->next;
            }
            regions_h = p->sprites_h;
        }
    }
    if (p->lines != 0) {
        text_h = Popup_StringPourCalculate(p);
        p->item_y += p->gap_y + text_h;
        p->edit_y += p->gap_y + text_h;
    } else
        p->line[0] = 0;
    if (p->items != 0) {
        rows = (p->cols + p->n_items - 1) / *max_ptr_1050_97d1((one = 1, &one), &p->cols);
        radio_h = p->row_h * rows;
        if ((p->flags & 0x1000) != 0)
            radio_h = p->list_h;
    }
    if (p->edit_items != 0)
        edits_h = Popup_edit_y_size(p) * p->n_edits;
    right_h = text_h + radio_h + edits_h;
    if (right_h != 0 && text_h != 0 && radio_h + edits_h != 0)
        right_h += p->gap_y;
    if (p->sprites == 0 || p->n_items == 0) {
        if (regions_h != 0 && right_h != 0) {
            p->text_x += p->gap_x;
            p->item_x += p->gap_x;
            p->edit_x += p->gap_x;
        }
    } else
        regions_h += right_h;
    p->box_h += *max_ptr_1050_97d1(&regions_h, &right_h);
    p->box_w += p->width;
    p->n_buttons = 0;
    if ((p->flags & 0x40) != 0) {
        p->btn[p->n_buttons].cmd = 1;
        p->n_buttons++;
    }
    if ((p->flags & 1) != 0) {
        p->btn[p->n_buttons].cmd = 2;
        p->n_buttons++;
    }
    p->btn[p->n_buttons].cmd = 0;
    p->n_buttons++;
    if (p->n_buttons != 0) {
        bx = p->box_w - p->gap_x;
        p->button_w = 0;
        for (i = 0; i < 3; i++)
            p->button_w = *max_ptr_1050_97d1(&p->button_w,
                           (label_w = Popup_button_x_size(p, g_popup_label[i]), &label_w));
        for (i = 0; i < p->n_extra; i++)
            p->button_w = *max_ptr_1050_97d1(&p->button_w,
                           (extra_w = Popup_button_x_size(p, p->extra_labels[i]), &extra_w));
        p->box_h += Popup_button_y_size(p) + p->gap_y * 2;
        for (i = p->n_buttons - 1; i >= 0; i--) {
            bx -= p->button_w;
            p->btn[i].x = bx;
            bx -= p->gap_x;
        }
        for (i = p->n_extra - 1; i >= 0; i--) {
            bx -= p->button_w;
            p->btn[p->n_buttons + i].x = bx;
            bx -= p->gap_x;
        }
        if (p->n_buttons + p->n_extra == 1 && p->n_items == 0 && p->n_edits == 0)
            p->btn[0].x = (p->box_w >> 1) - (p->button_w >> 1);
    }
    p->box_h = *max_ptr_1050_97d1(&p->box_h, &p->min_height);
    p->button_y = p->box_h - (Popup_button_y_size(p) + p->pad_y);
    MSGetDesktopSize(&desk);
    if (rect_width_1050_830c(&desk) <= 0x280)
        p->pos_x = (rect_width_1050_830c(&desk) >> 1) - (p->box_w >> 1);
    if (rect_height_1050_832b(&desk) <= 0x1e0)
        p->pos_y = (rect_height_1050_832b(&desk) >> 1) - (p->box_h >> 1);
    p->item_x += p->org_x;
    p->item_y += p->org_y;
    p->text_x += p->org_x;
    p->text_y += p->org_y;
    p->edit_x += p->org_x;
    p->edit_y += p->org_y;
    p->button_y += p->org_y;
    for (i = 0; i < p->n_buttons + p->n_extra; i++)
        p->btn[i].x += p->org_x;
    if (g_popup_dude != -1) {
        p->win_w = p->box_w + 0x14;
        Sprite_Init(&p->portrait, g_popup_dude + 0xd2);
        Sprite_Init(&p->tile, g_popup_dude + 0x69);
        CTable_Init(&g_game_palette, g_popup_dude + 0xd2);
        p->win_h = -g_frame_top_text[g_popup_dude] + p->box_h + 0x14;
        if (g_popup_dude == 1)
            p->win_w += 0x32;
        p->box_x = 0xa;
        p->box_y = -g_frame_top_text[g_popup_dude] + 0xa;
        if (rect_height_1050_83da(&p->portrait) + 0x14 > p->win_h)
            p->win_h = rect_height_1050_83da(&p->portrait) + 0x14;
        p->framed = 1;
        p->win_x = (rect_width_1050_830c(&desk) >> 1) - (p->win_w >> 1);
        p->win_y = (rect_height_1050_832b(&desk) >> 1) - (p->win_h >> 1);
    } else if (g_popup_tribe != -1) {
        if (g_popup_tribe == 8) {
            Sprite_Init(&p->portrait, 0x7d);
            Sprite_Init(&p->tile, 0x6f);
            CTable_Init(&g_game_palette, 0x6f);
            p->win_w = rect_width_1050_841c(&p->portrait) + p->box_w + 0x14;
            p->win_h = *max_ptr_1050_97d1((top_h = rect_height_1050_83da(&p->portrait) + 0x14, &top_h),
                        (popup_h = p->box_h + 0x14, &popup_h));
            p->box_x = rect_width_1050_841c(&p->portrait) + 0xa;
            p->box_y = (p->win_h >> 1) - (p->box_h >> 1) + 0xa;
            p->framed = 1;
            p->win_x = (rect_width_1050_830c(&desk) >> 1) - (p->win_w >> 1);
            p->win_y = (rect_height_1050_832b(&desk) >> 1) - (p->win_h >> 1);
        } else {
            anger = piss_factor(pissed(g_popup_tribe, g_game.player_nation));
            Sprite_Init(&p->portrait, (g_popup_tribe << 2) + anger + 0x104);
            Sprite_Init(&p->tile, g_popup_tribe + 0x73);
            CTable_Init(&g_game_palette, (g_popup_tribe << 2) + anger + 0x104);
            p->win_w = rect_width_1050_841c(&p->portrait) + p->box_w + 0x14;
            p->win_h = *max_ptr_1050_97d1((top_h2 = rect_height_1050_83da(&p->portrait) + 0x14, &top_h2),
                        (popup_h2 = p->box_h + 0x14, &popup_h2));
            if (g_popup_tribe == 5) {
                p->box_x = 0xd2;
                p->win_w -= 0x54;
            } else if (g_popup_tribe == 4)
                p->box_x = 0xa;
            else
                p->box_x = rect_width_1050_841c(&p->portrait) + 0xa;
            p->box_y = (p->win_h >> 1) - (p->box_h >> 1) + 0xa;
            p->framed = 1;
            p->win_x = (rect_width_1050_830c(&desk) >> 1) - (p->win_w >> 1);
            p->win_y = (rect_height_1050_832b(&desk) >> 1) - (p->win_h >> 1);
        }
    } else if (g_popup_nation != -1) {
        Sprite_Init(&p->portrait, g_popup_nation + 0xdc);
        Sprite_Init(&p->tile, g_popup_nation + 0x64);
        CTable_Init(&g_game_palette, g_popup_nation + 0xdc);
        p->win_w = p->box_w + 0x14;
        p->win_h = -g_frame_top_sel[g_popup_nation] + p->box_h + 0x14;
        if (g_popup_nation == 0)
            p->win_w += 0x14;
        p->box_x = 0xa;
        p->box_y = -g_frame_top_sel[g_popup_nation] + 0xa;
        if (rect_height_1050_83da(&p->portrait) > p->win_h)
            p->win_h = rect_height_1050_83da(&p->portrait);
        p->framed = 1;
        p->win_x = (rect_width_1050_830c(&desk) >> 1) - (p->win_w >> 1);
        p->win_y = (rect_height_1050_832b(&desk) >> 1) - (p->win_h >> 1);
    } else {
        p->win_x = p->pos_x;
        p->win_y = p->pos_y;
        p->win_w = p->box_w;
        p->win_h = p->box_h;
    }
    ret = 0;
done:
    return ret;
}

