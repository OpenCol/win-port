/* SetDifficulty_DrawIt: candidate for 1030:097b (616 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetDifficulty_DrawIt
 * size-source: 616, not the census 606 -- the last ten bytes are the switch's five-entry
 *   jump table.
 *
 * `SetNations_DrawIt`'s twin, one screen along: "Choose Difficulty Level" instead of "Select
 * European Power", five boxes instead of four, and the saved port at +0x134 instead of +0x12c.
 * Everything else is the same shape -- repaint from the saved port, a sunken bevel round the
 * same 160x120 box at the same place, the heading in two lines, the chosen box outlined in its
 * own colour with a name above and a line below.
 *
 * THE COLOURS ARE A LOCAL ARRAY AND NOT A GLOBAL ONE. Where the nations screen indexes the
 * table at DGROUP:0x99, this one carries `{0xfa, 0xfc, 0x0c, 0x0d, 0xf9}` as an initialised
 * auto array, copied out of a DGROUP template by two word moves and a byte move.
 *
 * THE DECLARATION IS INSIDE AN INNER BLOCK, and the shipped bytes are what say so: the copy
 * lands AFTER the switch, where a function-scope declaration would put it before the first
 * statement. Five lines further up and the whole tail is ten bytes out of place.
 *
 * `cols[s->level - 1]` is indexed off the local's own address -- `LEA AX,[BP-0x10] / ADD AX,
 * level / MOV AL,SS:[BX-1]` -- the same minus-one-into-the-base fold the nations screen does
 * on its table.
 *
 * `g_level_words` (SEG20:0xde10) is a RUN inside `g_misc_words`, starting at index 164, and
 * the MISC resource says what it holds: "Level", then "Easiest", "Easy", "Moderate", "Tough",
 * "Toughest". So `g_level_words[0]` ("Level") is NOT drawn -- the heading is the literals "Choose" and
 * "Difficulty Level" -- and [1..5] are the five difficulty names, which is why it is indexed by `s->level` rather than read as one word.
 */

#include "colonization.h"

#include "w16types.h"
#include "misc.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SetDifficulty, Font removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-








extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void FrameInfoRect(GRPort *s, RECT *r, int depth, int light, int dark);

extern void GRPort_String(GRPort *port, unsigned *font, char *t,
                                     int x, int y, int align);
extern void GRPort_DrawRect(GRPort *s, RECT *r);

/* same object: 4-byte `push cs; call ` (rule 47) */

void SetDifficulty_DrawIt(SetDifficulty *s)
{
    RECT r;
    char buf[0x50];
    register int x;
    register int y;

    SetRect(&r, 0, 0, 0x280, 0x1e0);
    GRPort_CopyToPort(&s->port, (GRPort *)s, &r, &r);
    SetRect(&r, 0, 0, 0xa0, 0x78);
    OffsetRect(&r, 0x20, 0x32);
    FrameInfoRect((GRPort *)s, &r, -2, 0xff, 0);
    switch (s->choice) {
    case 1:
        CopyRect(&r, &s->boxes[0]);
        break;
    case 2:
        CopyRect(&r, &s->boxes[1]);
        break;
    case 3:
        CopyRect(&r, &s->boxes[2]);
        break;
    case 4:
        CopyRect(&r, &s->boxes[3]);
        break;
    case 5:
        CopyRect(&r, &s->boxes[4]);
        break;
    }
    {
    unsigned char cols[5] = { 0xfa, 0xfc, 0x0c, 0x0d, 0xf9 };

    x = 0x70;
    y = 0x3c;
    TextColor(0xfa);
    GRPort_String((GRPort *)s, (unsigned *)&g_font_small, "Choose", x, y, 0x10);
    GRPort_String((GRPort *)s, (unsigned *)&g_font_small, "Difficulty Level", x, y + 0x19, 0x10);
    TextColor(cols[s->choice - 1]);
    GRPort_DrawRect((GRPort *)s, &r);
    x = (rect_width_1030_e4e5(&r) >> 1) + r.left;
    y = r.top + 0x12;
    buf[0] = 0;
    say_string(buf, g_difficulty_words[s->choice - 1]);
    STRUPR(buf);
    GRPort_String((GRPort *)s, (unsigned *)&g_heading_font, buf, x, y, 0x10);
    y = r.bottom - 0x12;
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_EASIEST - 1 + s->choice]);
    GRPort_String((GRPort *)s, (unsigned *)&g_heading_font, buf, x, y, 0x10);
    copy_all_to_screen_1030_e504(s);
    }
}
