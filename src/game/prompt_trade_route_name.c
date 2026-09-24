/* prompt_trade_route_name -- shipped at 1058:8868.
 *
 * Ported by tools/port.py from matched/game/prompt_trade_route_name.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_trade_name_busy;                        DGROUP:0x2668
 *   extern char far g_font_small;                        DGROUP:0x4b1e
 *   extern char far * far g_cur_route_rec;               DGROUP:0x4c18
 *   extern char far g_text_ctx;                          DGROUP:0x009e
 *   extern char far g_name_slots[][0x50];                SEG31:0x00a4
 *   extern char far g_game_record;                       SEG20:0x05ca
 *
 * The file's own ReportWin, WordPair dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 font_height_1058_be4f(WordPair *p);
void MSDoAllTasks_1058_bc15(void);
void route_strip_click(col_i16 x, col_i16 y);

extern char * current_window(void);







extern void set_long_24fc(void *w);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void window_relayout(void *o);
/* Two same-module helpers are defined here because the shipped calls to them are the
 * compiler's own `push cs / call ` (4 bytes): only a callee defined in the translation
 * unit gets that form, and the `jl` over the name-line block only stays short with it.
 * font_height_1058_be4f (1058:be4f, 14 bytes) reads the word at +2 of an object; MSDoAllTasks_1058_bc15
 * (1058:bc15, 14 bytes) is MSDoAllTasks behind a call. */
extern void MSDoAllTasks(void);




extern col_i16 prompt_text_loop(void *ctx, char *name, col_i16 max, char *buf);

void prompt_trade_route_name(col_i16 x, col_i16 y)
{
    ReportWin *o;
    char *w;
    char buf[0x50];

    if (g_trade_name_busy != 0)
        return;
    g_trade_name_busy = 1;
    w = current_window();
    o = (ReportWin *)(w != 0 ? w - 0x26 : w);
    if (o->ox > x || o->right < x || o->oy > y || o->bottom < y)
        goto done;
    x = x - o->ox;
    y = y - o->oy;
    set_long_24fc(o != 0 ? (char *)o + 0x26 : (char *)o);
    if (y >= 0x69 && y < 0x195) {
        route_strip_click(x, y);
        window_relayout(o);
        goto done;
    }
    if (y >= 0x19) {
        if (font_height_1058_be4f(&(*g_font_small)) * 2 + 0x1d > y) {
            strcpy(buf, g_cur_route_rec);
            if (prompt_text_loop(&(*g_text_ctx), "TRADENAME", 0x1f, buf) != 0)
                goto done;
            strcpy(g_cur_route_rec, g_name_slots[0]);
            window_relayout(o);
            goto done;
        }
    }
done:
    g_trade_name_busy = 0;
    MSDoAllTasks_1058_bc15();
    set_long_24fc(&g_game_record);
}
