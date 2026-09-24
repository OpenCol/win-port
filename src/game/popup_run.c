/* popup_run -- shipped at 1050:50c4.
 *
 * Ported by tools/port.py from matched/game/popup_run.c.
 *
 * STRCMP -> strcmp: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_cur_dialog;                       DGROUP:0x2500
 *   extern void far *g_popup_owner;                      DGROUP:0x24fc
 *   extern int far g_sel_c;                              DGROUP:0x2518
 *   extern int far g_text_value;                         DGROUP:0x251a
 *   extern int far g_sel_b;                              DGROUP:0x251c
 *   extern char far g_report_window;                     SEG20:0x1d06
 *   extern char far g_report_dialog;                     SEG20:0x1d2c
 *   extern char far g_game_palette;                      SEG20:0x01ac
 *   extern char far g_edit_bufs[][0x50];                 SEG31:0x00a4
 *
 * The file's own Item, Edit, Check, Popup dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_1e_1050_8d83(Check *c);
col_i16 get_w_77a(void *r);
col_u32 MSGetTicks_1050_8cf9(void);
void MSDoAllTasks_1050_8be7(void);
void MSDoTask_1050_8d23(void);
void MSHideWindow_of_1050_8cd8(void *o);
void close_dialog_at_26_1050_8bc8(void *o);
void ctl_get_text(Edit *e, char *buf);
void ctl_sel_text(void *c, char *buf);
void draw_anim_frame(void *w, col_i16 n);
void listbox_get_text_1050_8d5d(void *c, col_i16 i, char *buf);
void peek_message_only_1050_8d10(void);
void popup_free_controls(Popup *p);
void popup_layout(void *p);
void win_disable_1050_8c93(void *o);
void win_enable_1050_82e1(void *w);

extern void pack_palette_rgb_fwd(void *pal, void *buf, col_i16 start, col_i16 count);
extern void sprite_draw(RECT *out, void *s, void *port, col_i16 x, col_i16 y);
extern void run_modal(void *w);

extern void palette_set_range(void *pal, col_i16 first, col_i16 count, void *rgb);

/* same object: 4-byte `push cs; call ` (rule 47) */

















col_i16 popup_run(Popup *p, char *text)
{
    col_i16 i;
    Item *it;
    col_i16 frame;
    col_i32 until;
    RECT out;
    char rgb[0x124];
    char sel[0x50];
    char name[0x50];
    register col_i16 x;
    register col_i16 y;

    if (g_cur_dialog != 0)
        return 0;
    win_disable_1050_8c93(g_popup_owner);
    if (get_w_77a(&g_report_window) != -1 && get_w_77a(&g_report_window) != 0x63) {
        pack_palette_rgb_fwd(&(*g_game_palette), rgb, 0x8e, 0x61);
        MSHideWindow_of_1050_8cd8(&g_report_dialog);
        MSDoAllTasks_1050_8be7();
    }
    popup_layout(p);
    if ((*((col_i16 *)((unsigned char *)(p) + 0x0))) & 8)
        goto done;
    if (g_sel_c == 8) {
        frame = 1;
        while (frame <= 5) {
            until = MSGetTicks_1050_8cf9() + 15;
            while ((col_i32)MSGetTicks_1050_8cf9() < until)
                ;
            for (y = 0x40; y <= 0xd2; y += 0x20)
                for (x = 0; x <= 0x60; x += 0x20)
                    sprite_draw(&out, ((char *)((unsigned char *)(p) + 0x4e)), (*((char * *)((unsigned char *)(p) + 0x0))), x, y);
            draw_anim_frame((*((char * *)((unsigned char *)(p) + 0x0))), frame++);
        }
    }
    run_modal((*((char * *)((unsigned char *)(p) + 0x0))) + 0x26);
    peek_message_only_1050_8d10();
    while (((*((col_i16 *)((unsigned char *)(p) + 0x0))) & 0x400) == 0)
        MSDoTask_1050_8d23();
    if (text != 0 && (*((Edit * *)((unsigned char *)(p) + 0x3a8))) != 0)
        ctl_get_text((*((Edit * *)((unsigned char *)(p) + 0x3a8))), text);
    if ((*((col_i16 *)((unsigned char *)(p) + 0x7a))) > 1) {
        for (i = 0; (*((col_i16 *)((unsigned char *)(p) + 0x7a))) > i; i++)
            ctl_get_text(&((Edit * *)((unsigned char *)(p) + 0x3a8))[i], g_edit_bufs[i]);
    }
    if ((*((col_i16 *)((unsigned char *)(p) + 0x0))) & 0x1000) {
        if ((*((col_i16 *)((unsigned char *)(p) + 0xa8))) == 0) {
            ctl_sel_text((*((void * *)((unsigned char *)(p) + 0x3b0))), sel);
            it = (*((Item * *)((unsigned char *)(p) + 0x36e)));
            i = 0;
            while (it != 0) {
                listbox_get_text_1050_8d5d((*((void * *)((unsigned char *)(p) + 0x3b0))), i, name);
                if (strcmp(sel, name) == 0)
                    (*((col_i16 *)((unsigned char *)(p) + 0xa8))) = (*((col_i16 *)((unsigned char *)(it) + 0x2)));
                it = (*((void * *)((unsigned char *)(it) + 0xa)));
                i++;
            }
        }
    } else if ((*((col_i16 *)((unsigned char *)(p) + 0x0))) & 4) {
        it = (*((Item * *)((unsigned char *)(p) + 0x36e)));
        i = 0;
        while (it != 0) {
            (*((col_i16 *)((unsigned char *)(it) + 0x0))) &= ~4;
            if (get_w_1e_1050_8d83(&((Check * *)((unsigned char *)(p) + 0x3a4))[i]))
                (*((col_i16 *)((unsigned char *)(it) + 0x0))) |= 4;
            i++;
            it = (*((void * *)((unsigned char *)(it) + 0xa)));
        }
    }
    if (((*((col_i16 *)((unsigned char *)(p) + 0x0))) & 0x800) == 0) {
        popup_free_controls(p);
        MSDoAllTasks_1050_8be7();
    }
    g_sel_c = g_text_value = g_sel_b = -1;
done:
    win_enable_1050_82e1(g_popup_owner);
    if (get_w_77a(&g_report_window) != -1) {
        palette_set_range(&(*g_game_palette), 0x8e, 0x61, rgb);
        close_dialog_at_26_1050_8bc8(&g_report_window);
        MSDoAllTasks_1050_8be7();
    }
    return (*((col_i16 *)((unsigned char *)(p) + 0xa8)));
}
