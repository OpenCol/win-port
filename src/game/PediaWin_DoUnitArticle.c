/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of MSGetTextLength_1060_6977, text_search removed: the header declares them from the definition (W1 step 3). */
/* PediaWin_DoUnitArticle: candidate for 1060:3a6d (2697 bytes).  Verdict: RELOCATABLE.
 * name-source: original
 *
 * THE UNIT TYPES ARTICLE of the Colonizopedia, index 1 of `PediaWin_DrawArticle`'s jump table. It
 * draws its illustrations by CREATING A REAL UNIT: `make_fake_unit_2` makes one at (-6, -6) owned by
 * the player, the article pokes its type and icon fields directly to get each picture drawn, and
 * `delete_unit` throws it away at the end. Everything the page shows is therefore the same
 * renderer the map uses.
 *
 * THE FREE COLONIST (type 0) gets the whole specialist gallery: the generic icon 0x1c, then icons
 * 0x19, 0x1a and 0x1b, then every icon from 0 to 0x16 except 0x12 and 0x13, sixteen to a row and
 * wrapping down by 32 pixels.
 *
 * A SOLDIER, PIONEER, MISSIONARY, DRAGOON OR SCOUT (types 1..5) gets two icons, the plain colonist and its own speciality, with
 * `job_of_unit` naming the speciality and 0x17 remapped to 0x15 on the way. A
 * PIECE OF ARTILLERY (type 0xb) gets two as well: itself, and itself with bit 7 of +4 set, which the
 * caption names the DAMAGED artillery.
 *
 * BELOW THE PICTURES comes the statistics line: the defence as the combat figure, and for artillery the attack bonus and the
 * damaged penalty in brackets, and for types 1 and 4 the veteran figure, then the movement divided by
 * three and the cargo capacity if it has any. The whole line is assembled in one 80-byte buffer through the
 * `say_string` / `append_*` family and drawn once.
 *
 * A SHIPPED BUG: the three icons 0x19, 0x1a and 0x1b in the Free Colonist gallery are drawn at the
 * SAME x. The loop increments the column counter but never advances the cursor, so the last one
 * covers the other two and the gallery is two icons short of what it means to show.
 *
 * WHAT THE BYTES SETTLED: the veteran figure is `attack * 3 >> 1` (the byte at +6) -- an arithmetic shift, not a divide,
 * so no `CWD`/`SUB` correction appears -- and Borland expands the `* 3` into `ADD DX,DX / ADD DX,AX`
 * rather than an `IMUL`. The ")" the artillery name line ends with is a literal one-character string
 * through `STRCAT`, where every other bracket in the function goes through `close_string`.
 */

#include "colonization.h"

#include "w16types.h"
#include "jobs.h"
#include "units.h"
#include "misc.h"
#include "classes.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void SetGlobalSpriteScale(int num, int den);
extern void open_string(char *s);
extern void close_string(char *s);
extern void space_string(char *buf);
extern void spacer(char *buf, int n);
extern void plus_string(char *buf);
extern void minus_string(char *s);
extern void colon_string(char *buf);



extern void draw_text_resource(void *target, char *name, void *rect);
extern int make_fake_unit_2(int kind, int tile_x, int tile_y, int owner);
extern void delete_unit(int u);
extern int job_of_unit(int u);
extern void PieceDraw(GRPort *port, int u, int flags, int x, int y,
                                         int span, int mode);

/* same object: 4-byte `push cs; call ` */
extern PediaEntry *PediaWin_CurrentEntry(PediaWin *o);

extern int font_height_1060_695e(Font *font);

void PediaWin_DoUnitArticle(PediaWin *w)
{
    register int u;
    register int x;
    int x0;
    int y;
    int job;
    int number;
    int col;
    int cls;
    PediaEntry *item;
    RECT rc;
    char buf[0x50];

    SetGlobalSpriteScale(1, 1);
    item = PediaWin_CurrentEntry(w);
    if (item == 0)
        return;
    number = item->index;
    u = make_fake_unit_2(0, -6, -6, g_game.player_nation);
    if (u < 0)
        return;
    g_units[u].orders = ORDERS_NONE;
    g_units[u].type = number;
    g_units[u].spec = JOB_COLONIST;
    y = 10;
    buf[0] = 0;
    open_string(buf);
    say_string(buf, g_unit_types[g_units[u].type].name);
    colon_string(buf);
    text_search("PEDIA", "PEDIA", 1);
    STRCAT(buf, g_text_buf);
    close_string(buf);
    TextColor(0xc);
    GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, port_width_1060_6886((GRPort *)w) / 2,
                         w->rect.top + y, 0);
    y += font_height_1060_695e((Font *)&g_heading_font) + 2;
    y += 0xc;
    x0 = 0x14;
    x = x0;
    if (g_units[u].type == UNIT_COLONISTS) {
        g_units[u].spec = JOB_COLONIST_ALT;
        PieceDraw((GRPort *)w, u, 1, w->rect.left + x, w->rect.top + y - 5, 0x10, 0x64);
        x += 0x20;
        col = 1;
        for (job = 0x19; job <= 0x1b; job++) {
            g_units[u].spec = (char)job;
            PieceDraw((GRPort *)w, u, 1, w->rect.left + x, w->rect.top + y - 5, 0x10, 0x64);
            col++;
        }
        for (job = 0; job <= 0x16; job++) {
            if (job == 0x13)
                continue;
            if (job == 0x12)
                continue;
            g_units[u].spec = (char)job;
            PieceDraw((GRPort *)w, u, 1, w->rect.left + x, w->rect.top + y - 5, 0x10, 0x64);
            x += 0x20;
            col++;
            if (col >= 0x11) {
                col = 0;
                x = x0;
                y += 0x20;
            }
        }
    } else if (g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS || g_units[u].type == UNIT_PIONEERS
               || g_units[u].type == UNIT_SCOUTS || g_units[u].type == UNIT_MISSIONARIES) {
        cls = job_of_unit(u);
        g_units[u].spec = JOB_COLONIST;
        PieceDraw((GRPort *)w, u, 1, w->rect.left + x, w->rect.top + y - 5, 0x10, 0x64);
        x += 0x20;
        g_units[u].spec = (char)cls;
        if (cls == 0x17)
            g_units[u].spec = JOB_SOLDIER;
        PieceDraw((GRPort *)w, u, 1, w->rect.left + x, w->rect.top + y - 5, 0x10, 0x64);
        x += 0x20;
    } else {
        PieceDraw((GRPort *)w, u, 1, w->rect.left + x, w->rect.top + y - 5, 0x10, 0x64);
        x += 0x20;
        if (g_units[u].type == UNIT_ARTILLERY) {
            g_units[u].flags2 |= 0x80;
            PieceDraw((GRPort *)w, u, 1, w->rect.left + x, w->rect.top + y - 5, 0x10, 0x64);
            x += 0x20;
            g_units[u].flags2 &= 0x7f;
        }
    }
    buf[0] = 0;
    say_string(buf, g_unit_types[g_units[u].type].name);
    if (g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS || g_units[u].type == UNIT_PIONEERS
        || g_units[u].type == UNIT_SCOUTS || g_units[u].type == UNIT_MISSIONARIES) {
        space_string(buf);
        open_string(buf);
        say_string(buf, g_misc_words[MISC_AND]);
        space_string(buf);
        say_string(buf, g_spec_recs[cls].expert);
        close_string(buf);
    }
    if (g_units[u].type == UNIT_ARTILLERY) {
        space_string(buf);
        open_string(buf);
        say_string(buf, g_misc_words[MISC_AND]);
        space_string(buf);
        say_string(buf, g_misc_words[MISC_DAMAGED]);
        space_string(buf);
        say_string(buf, g_unit_types[g_units[u].type].name);
        STRCAT(buf, ")");
    }
    TextColor(0xff);
    GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x + 0xf, w->rect.top + y + 0xa, 1);
    y += font_height_1060_695e((Font *)&g_heading_font) + 2;
    x += MSGetTextLength_1060_6977(&g_heading_font, buf);
    y += 0x24;
    x = x0;
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_COMBAT]);
    colon_string(buf);
    say_number(buf, g_unit_types[g_units[u].type].defence);
    if (g_units[u].type == UNIT_ARTILLERY) {
        spacer(buf, 3);
        open_string(buf);
        say_string(buf, g_misc_words[MISC_ATTACK]);
        colon_string(buf);
        plus_string(buf);
        say_number(buf, g_unit_types[g_units[u].type].attack
                        - g_unit_types[g_units[u].type].defence);
        space_string(buf);
        say_string(buf, g_misc_words[MISC_DAMAGED]);
        colon_string(buf);
        minus_string(buf);
        say_number(buf, 2);
        close_string(buf);
    } else if (g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS) {
        spacer(buf, 3);
        open_string(buf);
        say_string(buf, g_misc_words[MISC_VETERAN]);
        colon_string(buf);
        say_number(buf, g_unit_types[g_units[u].type].attack * 3 >> 1);
        close_string(buf);
    }
    spacer(buf, 3);
    say_string(buf, g_misc_words[MISC_MOVES]);
    colon_string(buf);
    say_number(buf, g_unit_types[g_units[u].type].moves / 3);
    if (g_unit_types[g_units[u].type].holds > 0) {
        spacer(buf, 3);
        open_string(buf);
        say_string(buf, g_misc_words[MISC_CARGO_HOLDS]);
        colon_string(buf);
        say_number(buf, g_unit_types[g_units[u].type].holds);
        close_string(buf);
    }
    TextColor(0xff);
    GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y, 1);
    y += font_height_1060_695e((Font *)&g_heading_font) + 2;
    STRCPY(buf, "UNIT");
    say_number(buf, number);
    SetRect(&rc, w->rect.left + 0xa, w->rect.top + y, w->rect.right - 0xa, w->rect.bottom - 5);
    set_wrap_font(&g_number_font);
    draw_text_resource(w, buf, &rc);
    delete_unit(g_game.unit_count - 1);
    SetGlobalSpriteScale(3, 4);
    SetGlobalSpriteScale(1, 1);
}
