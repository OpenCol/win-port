/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 160 of the imported file. */
/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of MSGetTextLength_1060_6977, text_search removed: the header declares them from the definition (W1 step 3). */
/* PediaWin_DoTerrainArticle: candidate for 1060:44f6 (3491 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: PediaWin_DoTerrainArticle
 * name-source: original
 *
 * THE TERRAIN ARTICLE of the Colonizopedia, and the largest of the six. It draws a picture of the
 * terrain, a row per commodity it yields, and its movement and defence figures, then lets the
 * TERRAIN<n> text resource fill whatever is left of the frame.
 *
 * THE PICTURE IS A THREE-BY-THREE PATCH of 32-pixel tiles built by LAYERING, not by using a
 * finished image. Every cell starts with the base art for the terrain, and then up to four more
 * sprites go on top of it depending on what the terrain is and WHICH CELL of the nine it is:
 *   - a forest, hills or mountains overlay chosen from the nine-byte pattern at DGROUP:0x28a8 --
 *     05 07 06 / 0d 0f 0e / 09 0b 0a, one variant per cell, so the patch tiles seamlessly;
 *   - on ordinary land, a tree in the four corners (a different one for terrain 1) and a hut on
 *     the bottom middle;
 *   - a river down the left column and a road down the right, each in two pieces;
 *   - and the bonus resource, if the terrain has one, in the middle cell only.
 * The terrain number decides all of this through four flags worked out once: hilly (mountains
 * 0x1b, hills 0x1c), water (ocean 0x19, sea lane 0x1a), arctic (0x18) and the two forested bands
 * 8..0x0f and 0x10..0x17. No river on hilly, water or arctic cells, no road on water. (Until
 * 2026-09-27 the three flags were named one place off: water, arctic, hills.) Hills and mountains
 * draw over base art 3; otherwise the base art is the terrain itself from 0x18 and `terrain & 7`
 * below it, which is what makes the twenty-four
 * land types eight terrains in three bands; a forested type whose base art would be 1 uses 0x11
 * instead and drops its overlay, the one exception in the table.
 *
 * A COMMODITY ROW is drawn only when the terrain yields that commodity at all. Each is the goods
 * sprite, the name, the yield -- plus one for goods 0 and 8, doubled for goods 5 -- and then the
 * plough, road or coast / river bonus, which is 1 plus one more for goods 4 and one more for goods 5. If
 * `resource_bonus` says the terrain's own resource affects this commodity, a second half-row
 * follows with that resource's sprite and either "x2" for a negative score or "+n" for a positive
 * one, doubled again for goods 5, and with a second figure after a slash for goods 0 and 8. The
 * row ends with the EXPERT bonus, +3 for goods 0 and 8 and x2 for everything else.
 * Everything is laid out by carrying one x along the row and adding each string's measured width,
 * so the columns are not columns at all.
 *
 * WHAT THE BYTES SETTLED: three doublings are `x = x * 2` and not `x = x + x`. Borland compiles the
 * multiply as `ADD AX,AX` and the addition as a reload of the same memory operand, so the two are
 * three bytes apart and the shipped code says which was written (now rule 85).
 */

#include "colonization.h"

#include "w16types.h"
#include "misc.h"
#include "labels.h"
#include "terrain.h"
/* PORT: this file's own Font, Screen, EventTable, EventWin, GRPort, GameWin, DlgControl, SImageButton, ColChild, SScrollBar, VListItem, VList, PediaWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"



class DlgControl;/* PORT: this file's own DialogWin removed: include/gen/classes.h define it once (W1 step 2). */








template <class T> class LinkedList : public VList { public: LinkedList(); };
/* PORT: PediaWin restored: classes.h has only its C view, and C++ files view it three ways (W1). */
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
#include "records.h"


   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

#include "icons.h"

extern void open_string(char *s);
extern void close_string(char *s);
extern void colon_string(char *buf);
extern void space_string(char *buf);
extern void spacer(char *buf, int n);
extern void plus_string(char *buf);
extern void times_string(char *buf);
extern void percent_string(char *buf);


extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void FrameInfoRect(GRPort *w, RECT *r, int depth, int light, int dark);

extern void draw_text_resource(void *target, char *name, void *rect);
extern int resource_bonus(int resource, int goods);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" PediaEntry *PediaWin_CurrentEntry(PediaWin *o);

extern "C" int font_height_1060_695e(Font *font);


extern "C"   /* PORT: C linkage: C files call it too (W2) */
void PediaWin_DoTerrainArticle(PediaWin *w)
{
    register int sx;
    register int sy;
    int x;
    int y;
    int cx;
    int terr;
    int col;
    int row;
    int x_end;
    int y_end;
    int ry;
    int art;
    int forested;
    int swapped;
    int bonus;
    int hilly;
    int water;
    int arctic;
    int job;
    int extra;
    int amt;
    PediaEntry *item;
    RECT r;
    Pattern pat;
    RECT r1;
    RECT r2;
    RECT r3;
    RECT r4;
    RECT r5;
    RECT r6;
    RECT r7;
    RECT r8;
    RECT r9;
    RECT r10;
    RECT r11;
    RECT r12;
    RECT r13;
    RECT r14;
    RECT r15;
    RECT r16;
    char buf[0x50];
    char name[0x28];

    pat = g_forest_pattern;
    SetGlobalSpriteScale(1, 1);
    item = PediaWin_CurrentEntry(w);
    if (item == 0)
        return;
    terr = item->index;
    y = 0xa;
    buf[0] = 0;
    open_string(buf);
    name[0] = 0;
    say_string(name, g_terrain_rows[terr].name);
    if (terr >= 8 && terr < 0x10) {
        space_string(name);
        say_string(name, g_feature_words[FEATURE_FOREST]);
    }
    STRCAT(buf, name);
    colon_string(buf);
    text_search((void*)("PEDIA"), "PEDIA", 2);
    STRCAT(buf, g_text_buf);
    close_string(buf);
    TextColor(0xc);
    GRPort_String(w, (unsigned *)&g_heading_font, buf, port_width_1060_6886((GRPort *)w) / 2,
                         w->rect.top + y, 0);
    y += font_height_1060_695e((Font *)&g_heading_font) + 2;
    x = 7;
    hilly = (terr == TERRAIN_MOUNTAINS || terr == TERRAIN_HILLS) ? 1 : 0;
    water = (terr == TERRAIN_OCEAN || terr == TERRAIN_SEA_LANE) ? 1 : 0;
    arctic = terr == TERRAIN_ARCTIC ? 1 : 0;
    if (hilly != 0)
        art = 3;
    else if (terr >= 0x18)
        art = terr;
    else
        art = terr & 7;
    swapped = 0;
    forested = ((terr >= 8 && terr < 0x10) || (terr >= 0x10 && terr < 0x18)) ? 1 : 0;
    if (forested != 0 && art == 1) {
        swapped = 1;
        art = 0x11;
    }
    bonus = g_terrain_bonus[terr];
    sx = x;
    x_end = x + 0x63;
    sy = y;
    y_end = y + 0x63;
    SetRect(&r, w->rect.left + sx + 1, w->rect.top + sy + 1, w->rect.left + x_end - 1, w->rect.top + y_end - 1);
    InflateRect(&r, 3, 3);
    FrameInfoRect(w, &r, 3, 7, 0xf8);
    for (row = 0; row < 3; row++) {
        sy = row * 0x20 + y + 2;
        for (col = 0; col < 3; col++) {
            sx = col * 0x20 + x + 2;
            Sprite_Draw(&r1, &g_art_base[art], w, w->rect.left + sx, w->rect.top + sy);
            if (forested != 0 && swapped == 0)
                Sprite_Draw(&r2, &g_terrain_sprites[TSPR_FOREST + pat.b[row * 3 + col]], w,
                            w->rect.left + sx, w->rect.top + sy);
            if (terr == TERRAIN_HILLS)
                Sprite_Draw(&r3, &g_terrain_sprites[TSPR_HILLS + pat.b[row * 3 + col]], w,
                            w->rect.left + sx, w->rect.top + sy);
            if (terr == TERRAIN_MOUNTAINS)
                Sprite_Draw(&r4, &g_terrain_sprites[TSPR_MOUNTAINS + pat.b[row * 3 + col]], w,
                            w->rect.left + sx, w->rect.top + sy);
            if (hilly == 0 && water == 0 && forested == 0 && arctic == 0) {
                if (col != 1 && row != 1) {
                    if (terr == 1)
                        Sprite_Draw(&r5, &g_art_base[17], w, w->rect.left + sx, w->rect.top + sy);
                    else
                        Sprite_Draw(&r6, &g_terrain_sprites[TSPR_FOREST], w, w->rect.left + sx, w->rect.top + sy);
                }
                if (col == 1 && row == 2)
                    Sprite_Draw(&r7, &g_terrain_sprites[150], w, w->rect.left + sx, w->rect.top + sy);
            }
            if (hilly == 0 && water == 0 && arctic == 0 && col == 0) {
                if (row == 1)
                    Sprite_Draw(&r8, &g_terrain_sprites[23], w, w->rect.left + sx, w->rect.top + sy);
                else if (row == 2)
                    Sprite_Draw(&r9, &g_terrain_sprites[27], w, w->rect.left + sx, w->rect.top + sy);
            }
            if (water == 0 && col == 2) {
                if (row == 0) {
                    Sprite_Draw(&r10, &g_terrain_sprites[83], w, w->rect.left + sx, w->rect.top + sy);
                    Sprite_Draw(&r11, &g_terrain_sprites[86], w, w->rect.left + sx, w->rect.top + sy);
                } else if (row == 1) {
                    Sprite_Draw(&r12, &g_terrain_sprites[82], w, w->rect.left + sx, w->rect.top + sy);
                    Sprite_Draw(&r13, &g_terrain_sprites[85], w, w->rect.left + sx, w->rect.top + sy);
                }
            }
            if (col == 1 && row == 1 && bonus != -1)
                Sprite_Draw(&r14, &g_terrain_sprites[TSPR_BONUS + bonus], w, w->rect.left + sx, w->rect.top + sy);
        }
    }
    x += 0x82;
    ry = y;
    for (job = 0; job < 9; job++) {
        if (g_terrain_rows[terr].yield[job] == 0)
            continue;
        cx = x;
        Sprite_Draw(&r15, &g_icon_sprites[ICON_EXPERT + job], w, w->rect.left + x, w->rect.top + ry);
        cx += 0x20;
        buf[0] = 0;
        say_string(buf, g_spec_recs[job].name);
        colon_string(buf);
        amt = g_terrain_rows[terr].yield[job];
        if (job == 0 || job == 8)
            amt++;
        if (job == 5)
            amt = amt * 2;
        say_number(buf, amt);
        TextColor(0xff);
        GRPort_String(w, (unsigned *)&g_info_font_large, buf, w->rect.left + cx, w->rect.top + ry + 0xc, 1);
        cx += MSGetTextLength_1060_6977(&g_info_font_large, buf);
        buf[0] = 0;
        spacer(buf, 4);
        if (job <= 3)
            say_string(buf, g_misc_words[MISC_PLOW]);
        else if (job < 8)
            say_string(buf, g_misc_words[MISC_ROAD]);
        else
            say_string(buf, g_misc_words[MISC_COAST]);
        STRCAT(buf, "/");
        say_string(buf, g_misc_words[MISC_RIVER]);
        colon_string(buf);
        plus_string(buf);
        say_number(buf, (job == 5) + (job == 4) + 1);
        TextColor(0xff);
        GRPort_String(w, (unsigned *)&g_info_font_large, buf, w->rect.left + cx, w->rect.top + ry + 0xc, 1);
        cx += MSGetTextLength_1060_6977(&g_info_font_large, buf);
        extra = resource_bonus(bonus, job);
        if (extra != 0) {
            buf[0] = 0;
            spacer(buf, 1);
            TextColor(0xff);
            GRPort_String(w, (unsigned *)&g_info_font_large, buf, w->rect.left + cx, w->rect.top + ry + 0xc, 1);
            cx += MSGetTextLength_1060_6977(&g_info_font_large, buf);
            Sprite_Draw(&r16, &g_terrain_sprites[TSPR_BONUS + bonus], w, w->rect.left + cx, w->rect.top + ry - 4);
            cx += 0x20;
            buf[0] = 0;
            if (bonus == 4)
                say_string(buf, g_misc_words[MISC_PRIME]);
            else
                say_string(buf, g_resource_name[bonus]);
            colon_string(buf);
            if (extra < 0) {
                times_string(buf);
                say_number(buf, 2);
            } else {
                if (job == 5)
                    extra = extra * 2;
                plus_string(buf);
                say_number(buf, extra);
                if (job == 8 || job == 0) {
                    STRCAT(buf, "/");
                    say_number(buf, extra * 2);
                }
            }
            TextColor(0xff);
            GRPort_String(w, (unsigned *)&g_info_font_large, buf, w->rect.left + cx, w->rect.top + ry + 0xc, 1);
            cx += MSGetTextLength_1060_6977(&g_info_font_large, buf);
        }
        buf[0] = 0;
        spacer(buf, 4);
        say_string(buf, g_misc_words[MISC_EXPERT]);
        colon_string(buf);
        if (job == 0 || job == 8) {
            plus_string(buf);
            say_number(buf, 3);
        } else {
            times_string(buf);
            say_number(buf, 2);
        }
        TextColor(0xff);
        GRPort_String(w, (unsigned *)&g_info_font_large, buf, w->rect.left + cx, w->rect.top + ry + 0xc, 1);
        cx += MSGetTextLength_1060_6977(&g_info_font_large, buf);
        ry += 0x21;
    }
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_MOVE_COST]);
    colon_string(buf);
    say_number(buf, g_terrain_rows[terr].movement);
    spacer(buf, 4);
    say_string(buf, g_misc_words[MISC_DEFENSE_OR_AMBUSH_BONUS]);
    colon_string(buf);
    plus_string(buf);
    say_number(buf, g_terrain_rows[terr].defence * 0x19);
    percent_string(buf);
    TextColor(0xff);
    GRPort_String(w, (unsigned *)&g_info_font_large, buf, w->rect.left + x + 0x20, w->rect.top + ry + 0xc, 1);
    y = port_height_1060_699d((GRPort *)w) / 2 - 0x14;
    STRCPY(buf, "TERRAIN");
    say_number(buf, terr);
    SetRect(&r, w->rect.left + 0xa, w->rect.top + y, w->rect.right - 0xa, w->rect.bottom - 5);
    set_wrap_font(&g_number_font);
    draw_text_resource(w, buf, &r);
    SetGlobalSpriteScale(1, 1);
}

