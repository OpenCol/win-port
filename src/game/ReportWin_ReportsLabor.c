/* PORT: this file's own declarations of MSGetTextLength, ReportWin_OnLaborClick removed: the header declares them from the definition (W1 step 3). */
/* ReportWin_ReportsLabor: candidate for 1058:2dcd (1072 bytes).  Verdict: RELOCATABLE.
 * symbol: ReportWin_ReportsLabor
 *
 * The labor advisor report: the title (word SEG20:0xdd2a) centred in colour 249; the 29
 * specialty counts (+0x872) gathered from every colonist of the nation's colonies and every
 * qualifying unit of the nation (the +0x1c count folded into +0x13); then, skipping
 * specialties 0x12, 0x13 and 0x17 (0x1c drawn in 0x13's place), each specialty's sprite in
 * two forms with its name (word table SEG20:0xd61c) and count in three columns of eight or
 * nine rows 37 apart, 210 wide, from (25, 40); the hit rectangles are recorded at +0x7fe /
 * +0x838 (off-window by default). The click handlers are then installed. The module's
 * wrappers are defined here because the shipped calls to them are the compiler's own
 * `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "units.h"
#include "misc.h"
#include "classes.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void SetGlobalSpriteScale(int num, int den);
extern void set_europe(int n);
extern void set_colony(int colony);

extern char *strings(int n);
extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);

extern int get_specialty(int u);
extern int is_colonist(int u);
extern int ColonistIcon(int spec);
extern RECT *Sprite_DrawMono(RECT *out, Sprite *sprite, GRPort *port, int x, int y, unsigned char mode);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);


extern void ReportWin_OnLaborMouseMove(void);

extern int port_width_1058_bcfb(GRPort *port);

extern int MSGetTextLength_1058_be29(Font *font, char *s);

extern int font_height_1058_be4f(Font *font);

extern void *set_on_mousemove_1058_bf13(EventTable *table, void *h);

extern void *set_on_lclick_1058_bdb7(EventTable *table, void *h);

void ReportWin_ReportsLabor(ReportWin *o, int nation)
{
    int x0;
    int y0;
    int x;
    int y;
    int tx;
    int ty;
    int u;
    int sp;
    int n;
    int col;
    int w;
    int colw;
    int rowh;
    RECT rc1;
    RECT rc2;
    int i;
    int s;

    SetGlobalSpriteScale(1, 1);
    set_europe(nation);
    TextColor(0xf9);
    GRPort_String((GRPort *)o, (unsigned *)&g_report_font, strings(g_misc_words[MISC_LABOR_ADVISOR_REPORT]), port_width_1058_be8e((GRPort *)o) / 2, o->rect.top + 18, 16);
    x0 = 25;
    y0 = 40;
    memset_rtl(o->counts, 0, 0x3a);
    for (i = 0; i < 29; i++) {
        o->cell_x[i] = rect_width_1058_bd2d(&o->rect);
        o->cell_y[i] = rect_height_1058_bd4c(&o->rect);
    }
    for (i = 0; g_game.colony_count > i; i++) {
        set_colony(i);
        if ((char)g_cur_colony_rec->nation != (char)nation)
            continue;
        for (u = 0; g_cur_colony_rec->pop > u; u++) {
            sp = get_specialty(u);
            o->counts[sp]++;
        }
    }
    for (s = 0; g_game.unit_count > s; s++) {
        if ((char)(g_units[s].flags & UNIT_NATION_MASK) != (char)nation)
            continue;
        if (is_colonist(s) != 0)
            o->counts[g_units[s].spec]++;
    }
    o->counts[0x13] += o->counts[0x1c];
    y = y0;
    x = x0;
    n = 0;
    col = 0;
    colw = 210;
    rowh = 37;
    for (i = 0; i < 29; i++) {
        s = i;
        if (s == 0x13)
            continue;
        if (s == 0x17)
            continue;
        if (s == 0x12)
            continue;
        if (s == 0x1c)
            s = 0x13;
        o->cell_x[s] = o->rect.left + x - 15;
        o->cell_y[s] = o->rect.top + y;
        TextColor(0);
        Sprite_DrawMono(&rc1, &g_icon_sprites[ColonistIcon(s)], (GRPort *)o, o->rect.left + x - 3, o->rect.top + y, 0);
        Sprite_Draw(&rc2, &g_icon_sprites[ColonistIcon(s)], (GRPort *)o, o->rect.left + x, o->rect.top + y);
        tx = x + 20;
        ty = y + 7;
        message_buf_clear();
        say_string(message_buf_1058_bec6(), g_spec_recs[s].expert);
        w = MSGetTextLength_1058_be29((Font *)&g_heading_font, message_buf_1058_bec6());
        TextColor(0);
        GRPort_String((GRPort *)o, (unsigned *)&g_heading_font, message_buf_1058_bec6(), o->rect.left + tx, o->rect.top + ty, 1);
        tx = tx + w;
        ty += font_height_1058_be4f((Font *)&g_heading_font) + 1;
        message_buf_clear();
        say_number_1058_bede(o->counts[s]);
        tx = x + 47;
        TextColor(0);
        GRPort_String((GRPort *)o, (unsigned *)&g_heading_font, message_buf_1058_bec6(), o->rect.left + tx, o->rect.top + ty, 0);
        y += rowh;
        n++;
        if ((col != 0) + 8 <= n && col < 2) {
            n = 0;
            col++;
            y = y0;
            x += colw;
        }
    }
    set_on_mousemove_1058_bf13((EventTable *)((char *)o + 0x33), (void *)ReportWin_OnLaborMouseMove);
    set_on_lclick_1058_bdb7((EventTable *)((char *)o + 0x33), (void *)ReportWin_OnLaborClick);
}
