/* PediaWin_CargoLine: candidate for 1060:358a (535 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * One row of the Colonizopedia's cargo article, at `y` below the frame: `PediaWin_DoCargoArticle`
 * calls it for each goods with the speciality that produces it, stepping `y` by 0x17.
 *
 * The icons come first, at three-quarter scale set through `SetGlobalSpriteScale(3, 4)` and
 * restored to 1:1 at the end: the expert's sprite when there is one, then the goods icon (unit
 * sprite `goods + 0x17`) once at full spacing and six more four pixels apart -- a stack. Then the
 * caption, white in the heading font: the goods name, and for a produced goods ` (<word> <expert
 * name>)`, the word from SEG20:0xde2c and the name from the speciality's 8-byte record (+2, the
 * same word `job_desc` returns).
 *
 * A NEGATIVE goods index is the row that is not a goods: icon 0x3a, speciality 8, and the name
 * word at SEG20:0xde2a -- the parameter `spec` is overwritten to get there.
 *
 * `x` and `i` are two `register` locals and take SI and DI in declaration order (rule 53).
 *
 * RENAMED 2026-09-26 from pedia_cargo_row: the Mac build's PediaWin::CargoLine(short, short, short): this plus goods, specialist and y. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "misc.h"
#include "classes.h"
#include "records.h"

#include "icons.h"

extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void space_string(char *buf);
extern void open_string(char *s);
extern void close_string(char *s);

extern void GRPort_String(GRPort *port, unsigned *font, const char *s,
                                     int x, int y, int align);
extern int write_in_color(char *s, int x, int y, int c);

void PediaWin_CargoLine(PediaWin *w, int goods, int spec, int y)
{
    int icon;
    int name;
    RECT r1;
    RECT r2;
    RECT r3;
    char buf[0x50];
    register int x;
    register int i;

    x = 10;
    icon = goods + 0x17;
    name = g_goods_name_word[goods];
    SetGlobalSpriteScale(3, 4);
    if (goods < 0) {
        icon = 0x3a;
        spec = 8;
        name = g_misc_words[MISC_FISH];
    }
    if (spec >= 0) {
        Sprite_Draw(&r1, &g_icon_sprites[ICON_EXPERT + spec], (GRPort *)w, w->rect.left + x, w->rect.top + y - 5);
        x += 0x20;
    }
    Sprite_Draw(&r2, &g_icon_sprites[icon], (GRPort *)w, w->rect.left + x, w->rect.top + y - 5);
    x += 0x20;
    for (i = 0; i < 6; i++) {
        Sprite_Draw(&r3, &g_icon_sprites[icon], (GRPort *)w, w->rect.left + x, w->rect.top + y - 5);
        x += 4;
    }
    x += 0xc;
    buf[0] = 0;
    say_string(buf, name);
    if (spec >= 0) {
        space_string(buf);
        open_string(buf);
        say_string(buf, g_misc_words[MISC_WITH]);
        space_string(buf);
        say_string(buf, g_spec_recs[spec].expert);
        close_string(buf);
    }
    TextColor(0xff);
    GRPort_String((GRPort *)w, (unsigned *)&g_heading_font, buf, w->rect.left + x, w->rect.top + y + 4, 1);
    write_in_color(buf, x, y + 4, g_colour_table[0]);
    SetGlobalSpriteScale(1, 1);
}
