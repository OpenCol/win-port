/* PediaWin_DoJobArticle: candidate for 1060:52b6 (1529 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: PediaWin_DoJobArticle
 * name-source: original
 *
 * THE COLONIST SKILLS ARTICLE of the Colonizopedia, index 3 of `PediaWin_DrawArticle`'s jump table.
 * Given a job it draws the specialist who does it, the buildings the job is done in, and what it
 * produces -- three rows of sprite-and-caption assembled left to right with the pen carried in a pair
 * of register variables.
 *
 * THE SPECIALIST is art tile job+0x52, except the twenty-eighth job which uses 0x43 instead, drawn at
 * the 8/7 sprite scale with the job's title beside it.
 *
 * THE BUILDING CHAIN is a linked walk, not a loop over a table: `building_of_job` gives the
 * first building that hosts the job, and each building record's +4 is the NEXT one, so a job done in
 * three buildings draws three rows and the list ends at the first negative. Each row is a sprite at
 * 0x15f plus the slot with the building's name after it, and the pen drops by the sprite's own height
 * plus four. The eleventh job (JOB_TOBACCONIST, 10) gets its first sprite lifted twenty pixels, which is a special case for
 * one tall building tile.
 *
 * WHAT IT PRODUCES is only drawn for jobs below 0x13: art tile job+0x17 with four remappings (jobs 8,
 * 0xd, 0x10 and 0x11 use 0x3a, 0x37, 0x39 and 0x3f), and the goods name from the goods-word table with
 * three more remappings, plus job 8 taking a word of its own from SEG20:0xde2a. The whole row is
 * placed at the x the building rows reached, which is remembered from the FIRST building row and left
 * at the heading's own x when there are no buildings at all.
 *
 * Finally the JOB<n> text fills the rest of the frame, with the vertical gap before it sixteen pixels
 * tighter (+4 then -20) when the job had buildings and twenty wider when it did not -- the two arms of one `if` that
 * both adjust the same pen.
 *
 * WHAT THE BYTES SETTLED: the sprite height is halved with `>> 1`, not `/ 2`, and the offset that
 * follows is `- 7` applied afterwards rather than folded. The shrink in the last `if` is two statements
 * -- `y += 4; y -= 20;` -- and not one expression, because the shipped adds and subtracts separately.
 */

#include "colonization.h"

#include "w16types.h"
#include "jobs.h"
#include "misc.h"
/* PORT: this file's own Font, Sprite, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, DlgControl, SImageButton, ColChild, SScrollBar, VListItem, VList removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a

#pragma option -a-



class DlgControl;








template <class T> class LinkedList : public VList { public: LinkedList(); };
#include "records.h"
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };

extern void open_string(char *s);
extern void close_string(char *s);
extern void colon_string(char *buf);

extern void text_search(void *ctx, char *section, int n);

extern void SetGlobalSpriteScale(int num, int den);
extern void Sprite_Init(Sprite *s, int n);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void GRPort_String(GRPort *port, unsigned *font, char *s,
                                     int x, int y, int size);

extern void draw_text_resource(void *target, char *name, void *rect);
extern int building_of_job(int job);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" PediaEntry *PediaWin_CurrentEntry(PediaWin *o);

extern "C" int font_height_1060_695e(Font *font);
extern "C" int MSGetTextLength_1060_6977(void *font, char *buf);
extern "C" int sprite_height_1060_69bc(Sprite *sprite);
extern "C" int sprite_width_1060_60c4(Sprite *sprite);

void PediaWin_DoJobArticle(PediaWin *w)
{
    register int x;
    register int y;
    int a;
    int job;
    int sx;
    int sy;
    int rx;
    int dy;
    int slot;
    int has;
    int pic;
    int h = 0;
    PediaEntry *item;
    RECT rc;
    RECT r1;
    RECT r2;
    RECT r3;
    RECT r4;
    char buf[0x50];
    Sprite s;

    SetGlobalSpriteScale(1, 1);
    item = PediaWin_CurrentEntry(w);
    if (item == 0)
        return;
    job = item->index;
    y = 0xa;
    buf[0] = 0;
    open_string(buf);
    say_string(buf, g_spec_recs[job].name);
    colon_string(buf);
    text_search("PEDIA", "PEDIA", 3);
    STRCAT(buf, g_text_buf);
    close_string(buf);
    TextColor(0xc);
    GRPort_String(w, (unsigned *)&g_heading_font, buf,
                         port_width_1060_6886((GRPort *)w) / 2, w->rect.top + y, 0);
    y += font_height_1060_695e((Font *)&g_heading_font) + 2;
    slot = building_of_job(job);
    y += 5;
    a = 0x14;
    if (slot < 0)
        y += 0xb;
    x = a;
    dy = 0;
    has = 0;
    if (slot >= 0) {
        has = 1;
        h = sprite_height_1060_69bc(&g_icon_sprites[slot]);
        dy = (h >> 1) - 7;
    }
    pic = job + 0x52;
    if (job == JOB_CONVERT)
        pic = 0x43;
    SetGlobalSpriteScale(8, 7);
    Sprite_Draw(&r1, &g_icon_sprites[pic], w, w->rect.left + x + 0xa, w->rect.top + y + dy - 0x16);
    x += 0x20;
    buf[0] = 0;
    say_string(buf, g_spec_recs[job].expert);
    TextColor(0xff);
    GRPort_String(w, (unsigned *)&g_heading_font, buf, w->rect.left + x + 3, w->rect.top + y + dy + 6, 1);
    x += MSGetTextLength_1060_6977(&g_heading_font, buf);
    x += 0x20;
    sx = x;
    sy = y;
    rx = sx;
    if (slot >= 0)
        rx = -1;
    while (slot >= 0) {
        x = sx;
        SetGlobalSpriteScale(1, 1);
        Sprite_Init(&s, slot + 0x15f);
        h = sprite_height_1060_69bc(&s);
        dy = (h >> 1) - 7;
        if (job == JOB_TOBACCONIST && y == sy)
            Sprite_Draw(&r2, &s, w, w->rect.left + x, w->rect.top + y - 0x14);
        else
            Sprite_Draw(&r3, &s, w, w->rect.left + x, w->rect.top + y);
        x += sprite_width_1060_60c4(&s) + 4;
        buf[0] = 0;
        say_string(buf, g_buildings[slot].name);
        TextColor(0xff);
        GRPort_String(w, (unsigned *)&g_info_font_large, buf, w->rect.left + x, w->rect.top + y + dy + 6, 1);
        x += MSGetTextLength_1060_6977(&g_info_font_large, buf);
        if (rx < 0)
            rx = x + 0x20;
        y += h + 4;
        slot = g_buildings[slot].next_level;
    }
    SetGlobalSpriteScale(8, 7);
    if (job < 0x13) {
        pic = job + 0x17;
        if (job == JOB_FISHERMAN)
            pic = 0x3a;
        if (job == JOB_CARPENTER)
            pic = 0x37;
        if (job == JOB_PREACHER)
            pic = 0x39;
        if (job == JOB_STATESMAN)
            pic = 0x3f;
        Sprite_Draw(&r4, &g_icon_sprites[pic], w, w->rect.left + rx, w->rect.top + sy + dy - 6);
        rx += 0x20;
        buf[0] = 0;
        pic = job;
        if (job == JOB_CARPENTER)
            pic = 0x10;
        if (job == JOB_PREACHER)
            pic = 0x11;
        if (job == JOB_STATESMAN)
            pic = 0x12;
        pic = g_goods_name_word[pic];
        if (job == JOB_FISHERMAN)
            pic = g_misc_words[MISC_FISH];
        say_string(buf, pic);
        TextColor(0xff);
        GRPort_String(w, (unsigned *)&g_number_font, buf, w->rect.left + rx, w->rect.top + sy + dy + 8, 1);
    }
    if (has != 0) {
        y += 4;
        y -= 0x14;
    } else
        y += 0x14;
    STRCPY(buf, "JOB");
    say_number(buf, job);
    SetRect(&rc, w->rect.left + 0xa, w->rect.top + y, w->rect.right - 10, w->rect.bottom - 5);
    set_wrap_font(&g_number_font);
    draw_text_resource(w, buf, &rc);
    SetGlobalSpriteScale(1, 1);
}

