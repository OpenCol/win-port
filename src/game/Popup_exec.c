/* PORT: this file's own declarations of DrawKingo, MSHideWindow_of_1050_8cd8, Popup_draw, ctl_get_text, ctl_sel_text, get_checked_1050_8d83, get_w_77a, listbox_get_text_1050_8d5d, win_disable_1050_8c93, win_enable_1050_82e1 removed: the header declares them from the definition (W1 step 3). */
/* Popup_exec: candidate for 1050:50c4 (901 bytes).  Verdict: RELOCATABLE.
 * size-source: 901, not the census 892. The trailing 9 bytes are the dead second epilogue.
 * name-source: sibling-build
 *
 * Runs a Popup and returns its result at +0xa8. Only one runs at a time: with a popup already
 * active (DGROUP:0x2500) the answer is 0 at once. The owner window is disabled for the duration,
 * and when the report window is showing a report other than 0x63, palette entries 0x8e..0xee are
 * saved and the report hidden. Unless flag 8 is set, it runs:
 *
 *   with DGROUP:0x2518 at 8, a five-frame animation first -- each frame a wait of 15 ticks,
 *   a 4 by 5 tiling of the popup's sprite at +0x4e, and `DrawKingo`;
 *   then the modal loop until flag 0x400 is set.
 *
 * Afterwards the text of the first edit field goes to `text` when there is one, and with more
 * than one field each field's text goes to its own 80-byte buffer at SEG31:0xa4. A list popup
 * (flag 0x1000) with no result yet takes the id of the item whose text equals the list box's
 * selection; a check-box popup (flag 4) copies each box's state into the 4 bit (bit 2) of its item's flags. Unless
 * flag 0x800 keeps it, the popup is torn down, and the three words at DGROUP:0x2518..0x251c are
 * reset to -1. Then the owner is enabled again, and the palette and the report restored.
 *
 * RENAMED 2026-09-26 from popup_run: the Mac build's Popup::exec(char*): this plus the text, running the popup and returning the choice. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
extern Popup *g_cur_dialog;                    /* DGROUP:0x2500 */

extern int g_popup_tribe;                     /* DGROUP:0x2518, declared */

extern int g_popup_nation;                     /* DGROUP:0x251c, declared */

struct logpal { char pad[1]; };
extern void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void EventWin_Modal(EventWin *w);

extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

/* same object: 4-byte `push cs; call ` (rule 47) */


extern void MSFlushInput_1050_8d10(void);

extern void Popup_close(Popup *popup);
extern void show_window_1050_8bc8(GameWin *o);

int Popup_exec(Popup *popup, char *text)
{
    int i;
    PopupItem *it;
    int frame;
    long until;
    RECT out;
    char rgb[0x124];
    char sel[0x50];
    char name[0x50];
    register int x;
    register int y;

    if (g_cur_dialog != 0)
        return 0;
    win_disable_1050_8c93((DialogWin *)g_popup_owner);
    if (get_w_77a(&g_report_window) != -1 && get_w_77a(&g_report_window) != 0x63) {
        CTable_CopyRawColors((CTable *)((struct logpal *)&g_game_palette), rgb, 0x8e, 0x61);
        MSHideWindow_of_1050_8cd8(&g_report_window_dlg);
        MSDoAllTasks_1050_8be7();
    }
    Popup_draw(popup);
    if (popup->flags & 8)
        goto done;
    if (g_popup_tribe == 8) {
        frame = 1;
        while (frame <= 5) {
            until = MSGetTicks_1050_8cf9() + 15;
            while ((long)MSGetTicks_1050_8cf9() < until)
                ;
            for (y = 0x40; y <= 0xd2; y += 0x20)
                for (x = 0; x <= 0x60; x += 0x20)
                    Sprite_Draw(&out, &popup->tile, (GRPort *)popup->win, x, y);
            DrawKingo(popup->win, frame++);
        }
    }
    EventWin_Modal((EventWin *)((char *)popup->win + 0x26));
    MSFlushInput_1050_8d10();
    while ((popup->flags & 0x400) == 0)
        MSDoTask_1050_8d23();
    if (text != 0 && popup->edit_boxes != 0)
        ctl_get_text(popup->edit_boxes, text);
    if (popup->n_edits > 1) {
        for (i = 0; popup->n_edits > i; i++)
            ctl_get_text(&popup->edit_boxes[i], g_name_slots[i]);
    }
    if (popup->flags & 0x1000) {
        if (popup->result == 0) {
            ctl_sel_text(popup->listbox, sel);
            it = popup->items;
            i = 0;
            while (it != 0) {
                listbox_get_text_1050_8d5d(popup->listbox, i, name);
                if (STRCMP(sel, name) == 0)
                    popup->result = it->id;
                it = it->next;
                i++;
            }
        }
    } else if (popup->flags & 4) {
        it = popup->items;
        i = 0;
        while (it != 0) {
            it->flags &= ~4;
            if (get_checked_1050_8d83(&popup->checks[i]))
                it->flags |= 4;
            i++;
            it = it->next;
        }
    }
    if ((popup->flags & 0x800) == 0) {
        Popup_close(popup);
        MSDoAllTasks_1050_8be7();
    }
    g_popup_tribe = g_popup_dude = g_popup_nation = -1;
done:
    win_enable_1050_82e1((DialogWin *)g_popup_owner);
    if (get_w_77a(&g_report_window) != -1) {
        CTable_Fill(&g_game_palette, 0x8e, 0x61, rgb);
        show_window_1050_8bc8((GameWin *)&g_report_window);
        MSDoAllTasks_1050_8be7();
    }
    return popup->result;
}
