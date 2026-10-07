/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of MSGetTextLength_1060_6977, Sprite_Init, text_search removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 109 of the imported file. */
/* PediaWin_DoBuildingsArticle: candidate for 1060:58cc (1533 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: PediaWin_DoBuildingsArticle
 *
 * THE BUILDINGS ARTICLE of the Colonizopedia, the sibling of `PediaWin_DoCargoArticle` and
 * `PediaWin_DoFathersArticle`. The heading is "(<building>: <PEDIA line 4>)" and under it a row that
 * reads left to right:
 *   - the building's own sprite (resource `id + 0x15f`), except for ids 0x10 and 0x1f, which have none
 *     and leave a 24-pixel gap;
 *   - the building's name;
 *   - when the building has a speciality (`job_of_building`, with 0x12 and 0x15 rejected), the
 *     expert's icon from SEG20:0x6c86 and name from the 8-byte records at SEG20:0xd61a, then the goods
 *     icon and the goods name. Three specialities point at a different goods and icon: 13 -> 0x10 with
 *     sprite 0x37, 0x10 -> 0x11 with 0x39, and 0x11 -> 0x12 with 0x3f.
 * The row is centred on the sprite: everything is offset by half its height less 7, floored at 0.
 *
 * A building that UPGRADES from another (the signed byte at +3 of its 12-byte record) gets a second line,
 * "<SEG20:0xde40>: <that building>", in the heading font.
 *
 * The rest of the frame is filled with the BUILDING<id> text, in the large font at DGROUP:0x4b16 for
 * ids 12, 13 and 14 -- which also start 15 pixels higher -- and the body font otherwise.
 *
 * The `x` register is ASSIGNED the width of that second line rather than advanced by it, and nothing
 * reads it afterwards. The upgrade test needs `(int)` on the signed byte (rule 51).
 */

#include "colonization.h"

#include "w16types.h"
#include "misc.h"
/* PORT: this file's own Font, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, DlgControl, SImageButton, ColChild, SScrollBar, VListItem, VList, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"



class DlgControl;








template <class T> class LinkedList : public VList { public: LinkedList(); };
#include "records.h"
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
#pragma option -a

#pragma option -a-

#include "icons.h"

extern void SetGlobalSpriteScale(int num, int den);
extern void open_string(char *s);
extern void close_string(char *s);
extern void colon_string(char *buf);


extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void draw_text_resource(void *target, char *name, void *rect);
extern int job_of_building(int building);

/* same object: 4-byte `push cs; call ` */
extern "C" PediaEntry *PediaWin_CurrentEntry(PediaWin *o);

extern "C" int font_height_1060_695e(Font *font);
extern "C" int sprite_width_1060_60c4(Sprite *sprite);
extern "C" int sprite_height_1060_69bc(Sprite *sprite);


extern "C"   /* PORT: C linkage: C files call it too (W2) */
void PediaWin_DoBuildingsArticle(PediaWin *o)
{
    Sprite spr;
    int x0;
    int idx;
    int sw;
    int sh;
    int ty;
    int job;
    int goods2;
    int icon;
    int dy;
    int sx;
    PediaEntry *item;
    RECT rc;
    RECT r1;
    RECT r2;
    RECT r3;
    char buf[0x50];
    register int y;
    register int x;

    SetGlobalSpriteScale(1, 1);
    item = PediaWin_CurrentEntry(o);
    if (item == 0)
        return;
    idx = item->index;
    y = 10;
    buf[0] = 0;
    open_string(buf);
    say_string(buf, g_buildings[idx].name);
    colon_string(buf);
    text_search((void*)("PEDIA"), "PEDIA", 4);
    STRCAT(buf, g_text_buf);
    close_string(buf);
    TextColor(12);
    GRPort_String(o, (unsigned *)&g_heading_font, buf, port_width_1060_6886(o) / 2, o->rect.top + y, 0);
    y += font_height_1060_695e((Font *)&g_heading_font) + 2;
    x0 = 10;
    if (idx == 0x10 || idx == 0x1f) {
        sw = 0;
        sh = 0x18;
        sx = 0;
    } else {
        Sprite_Init(&spr, idx + 0x15f);
        sw = sprite_width_1060_60c4(&spr);
        sh = sprite_height_1060_69bc(&spr);
        Sprite_Draw(&r1, &spr, o, o->rect.left + x0, o->rect.top + y);
        sx = sw + 10;
    }
    dy = (sh >> 1) - 7;
    dy = max_int_1060_63f4(0, dy);
    x = x0;
    ty = y;
    ty += dy;
    x += sx;
    buf[0] = 0;
    say_string(buf, g_buildings[idx].name);
    TextColor(0xff);
    GRPort_String(o, (unsigned *)&g_number_font, buf, o->rect.left + x, o->rect.top + ty + 6, 1);
    x += MSGetTextLength_1060_6977(&g_number_font, buf);
    x += 0x18;
    job = job_of_building(idx);
    if (job == 0x12 || job == 0x15)
        job = -1;
    if (job >= 0) {
        Sprite_Draw(&r2, &g_icon_sprites[ICON_EXPERT + job], o, o->rect.left + x, o->rect.top + ty - 10);
        x += 0x20;
        buf[0] = 0;
        say_string(buf, g_spec_recs[job].expert);
        TextColor(0xff);
        GRPort_String(o, (unsigned *)&g_number_font, buf, o->rect.left + x, o->rect.top + ty + 6, 1);
        x += MSGetTextLength_1060_6977(&g_number_font, buf);
        x += 0x18;
        goods2 = job;
        icon = goods2 + 0x17;
        if (job == 13) {
            goods2 = 0x10;
            icon = 0x37;
        }
        if (job == 0x10) {
            goods2 = 0x11;
            icon = 0x39;
        }
        if (job == 0x11) {
            goods2 = 0x12;
            icon = 0x3f;
        }
        Sprite_Draw(&r3, &g_icon_sprites[icon], o, o->rect.left + x, o->rect.top + ty - 10);
        x += 0x20;
        buf[0] = 0;
        say_string(buf, g_goods_name_word[goods2]);
        TextColor(0xff);
        GRPort_String(o, (unsigned *)&g_number_font, buf, o->rect.left + x, o->rect.top + ty + 6, 1);
        x += MSGetTextLength_1060_6977(&g_number_font, buf);
    }
    y += sh + 0x12;
    if ((int)g_buildings[idx].prev_level >= 0) {
        buf[0] = 0;
        say_string(buf, g_misc_words[MISC_PREREQUISITE]);
        colon_string(buf);
        say_string(buf, g_buildings[g_buildings[idx].prev_level].name);
        TextColor(0xff);
        GRPort_String(o, (unsigned *)&g_heading_font, buf, o->rect.left + x0, o->rect.top + y, 1);
        x = MSGetTextLength_1060_6977(&g_heading_font, buf);
        y += 0x14;
    }
    STRCPY(buf, "BUILDING");
    say_number(buf, idx);
    if (idx == 12 || idx == 13 || idx == 14) {
        set_wrap_font(&g_info_font_large);
        y -= 0xf;
    } else {
        set_wrap_font(&g_number_font);
    }
    SetRect(&rc, o->rect.left + 10, o->rect.top + y, o->rect.right - 10, o->rect.bottom - 5);
    draw_text_resource(o, buf, &rc);
    set_wrap_font(&g_number_font);
    SetGlobalSpriteScale(1, 1);
}

