/* SetNations_DrawIt: candidate for 1030:00b7 (595 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetNations_DrawIt
 * size-source: 595, not the census 587 -- the last eight bytes are the switch's four-entry
 *   jump table and its padding.
 *
 * Redraws the "Select European Power" screen. The whole 640x480 saved port at +0x12c is blitted
 * back first, so the screen is repainted from scratch every time the highlight moves; then a
 * sunken bevel round the 160x120 box at (0x20, 0x32), the heading in two lines, and the chosen
 * nation's box outlined in its own colour with its name above and its power's advantage word below (MISC 173..176: Immigration, Cooperation, Conquest, Trade).
 *
 * THE FOUR BOXES ARE A SWITCH AND NOT AN ARRAY INDEX. `choice` runs 1 to 4 and each case copies
 * one of the four rectangles at +0xc4, +0xcc, +0xd4 and +0xdc -- consecutive and eight bytes
 * apart, so `boxes[choice - 1]` would have done, and the shipped spells it out four times.
 *
 * The highlight colour comes from the five-byte table at DGROUP:0x99 indexed by `choice`
 * itself, so entry 0 is the unused no-selection slot and 1..4 are the nations' colours.
 *
 * `TextColor` takes an `unsigned char` in this module, and both call sites prove it: the
 * literal 0xfa is pushed as the word `68 FA 00` where a SIGNED char would truncate to -6 and
 * give `6A FA`, and the table byte is pushed straight out of AL with AH never set, which an
 * `int` parameter would have to widen first (rule 55).
 *
 * `g_nation_name[s->choice - 1]` is one index off the table and Borland folds the minus one
 * into the BASE -- `MOV DX,offset / ADD DX,-2` -- rather than into the displacement, which is
 * what `(g_nation_name - 1)[s->choice]` gives instead.
 *
 * `g_power_words` (SEG20:0xde20) is a RUN inside `g_misc_words`, starting at index 172:
 * "Power", then "Immigration", "Cooperation", "Conquest", "Trade" -- the heading and the four
 * European powers' advantages, in nation order. MISC index 171 just before it is
 * "European Power", which is this screen's title.
 */

#include "colonization.h"

#include "w16types.h"
#include "misc.h"
class DlgControl { public: short kind; short id; void *win; RECT rect; short hwnd; DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; DlgControl(); ~DlgControl(); };
class SButton : public DlgControl { public: short enabled; long action; short colour; SButton(); ~SButton(); };
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class SetNations : public GameWin { public: RECT boxes[4]; int hit; int choice; SButton ok; SButton cancel; GRPort port; int done; int cancelled; SetNations(); ~SetNations(); };
typedef struct { short hfont, height; } Font;

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void FrameInfoRect(GRPort *s, RECT *r, int depth, int light, int dark);

extern void GRPort_String(GRPort *port, unsigned *font, char *t,
                                     int x, int y, int align);
extern void GRPort_DrawRect(GRPort *s, RECT *r);

/* same object: 4-byte `push cs; call ` (rule 47) */

void SetNations_DrawIt(SetNations *s)
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
    SetRect(&r, 0, 0, 0x280, 0x1e0);
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
    }
    TextColor(0xfa);
    x = 0x70;
    y = 0x3c;
    GRPort_String((GRPort *)s, (unsigned *)&g_font_small, "Select", x, y, 0x10);
    GRPort_String((GRPort *)s, (unsigned *)&g_font_small, "European Power", x, y + 0x19, 0x10);
    TextColor(g_nation_colour[s->choice - 1]);
    GRPort_DrawRect((GRPort *)s, &r);
    x = (rect_width_1030_e4e5(&r) >> 1) + r.left;
    y = r.top + 0x12;
    buf[0] = 0;
    say_string(buf, g_nation_name[s->choice - 1]);
    STRUPR(buf);
    GRPort_String((GRPort *)s, (unsigned *)&g_heading_font, buf, x, y, 0x10);
    y = r.bottom - 0x12;
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_IMMIGRATION - 1 + s->choice]);
    GRPort_String((GRPort *)s, (unsigned *)&g_heading_font, buf, x, y, 0x10);
    copy_all_to_screen_1030_e504(s);
}
