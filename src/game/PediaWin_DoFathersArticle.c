/* PediaWin_DoFathersArticle: candidate for 1060:5ec9 (415 bytes).  Verdict: RELOCATABLE.
 * symbol: PediaWin_DoFathersArticle
 *
 * Draws a Founding Father article: the selected list item's father index (+4) names the
 * father's own name, "(<name>: ...)", from the name word of the 6-byte FATHER table at SEG20:0xd6fa, the fifth PEDIA
 * text line and the suffix at data segment 29 +0x3c; the heading is drawn shadowed in
 * colour 12, centred at half the window's width, and below it the FATHER<n> text resource is
 * laid out in the window rectangle inset 10/5. The sprite scale is set to 1:1 before and after it.
 *
 * The module's -called wrappers are defined first because the shipped calls to them are
 * the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern void SetGlobalSpriteScale(int num, int den);
extern void *VList_Head(void *list);
extern void *VList_Next(void *list);
extern void open_string(char *s);
extern void close_string(char *s);
extern void colon_string(char *buf);

extern void text_search(void *ctx, char *section, int n);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);

extern void draw_text_resource(void *target, char *name, void *rect);

extern void *VList_Head_1060_68db(void *list);

extern void *VList_Next_1060_68fb(void *list);

extern PediaEntry *PediaWin_CurrentEntry(PediaWin *o);

extern int port_width_1060_65ca(GRPort *port);

extern int font_height_1060_695e(Font *font);

void PediaWin_DoFathersArticle(PediaWin *o)
{
    PediaEntry *item;
    RECT rc;
    char buf[0x50];
    int father;
    int y;

    SetGlobalSpriteScale(1, 1);
    item = PediaWin_CurrentEntry(o);
    if (item == 0)
        return;
    father = item->index;
    y = 10;
    buf[0] = 0;
    open_string(buf);
    say_string(buf, g_fathers[father].name);
    colon_string(buf);
    text_search("PEDIA", "PEDIA", 5);
    STRCAT(buf, g_text_buf);
    close_string(buf);
    TextColor(12);
    GRPort_String((GRPort *)o, (unsigned *)&g_heading_font, buf, port_width_1060_6886((GRPort *)o) / 2, o->rect.top + y, 0);
    y += font_height_1060_695e((Font *)&g_heading_font) + 2;
    y += 12;
    STRCPY(buf, "FATHER");
    say_number(buf, father);
    SetRect(&rc, o->rect.left + 10, o->rect.top + y, o->rect.right - 10, o->rect.bottom - 5);
    set_wrap_font(&g_heading_font);
    draw_text_resource(o, buf, &rc);
    SetGlobalSpriteScale(1, 1);
}
