/* ask_choice: candidate for 1068:cd4a (1272 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ask_choice
 * name-source: descriptive
 *
 * THE MULTIPLE-CHOICE DIALOG: a message, a radio list of options, an OK and a Cancel button,
 * run modally, returning the index the radio set left at DGROUP:0x56b4 -- or -1, which
 * `dialog_ok_cancel` writes there when Cancel is pressed. Six destructible objects
 * live in the frame at once (a colour table, a font, the window, two buttons and the radio
 * set), which is what the `6` handed to `__InitExceptBlock_rtl` counts.
 *
 * THE LAYOUT is derived entirely from the two strings. `text_widest_line` measures the
 * options and gives back both a line count and the widest line; the width plus 0x20 becomes the
 * radio column, plus another 0x5a the window. The message is then wrapped to that width plus
 * 0x46 (it is the radio list that is cut 0x46 short, for the buttons) -- and its line count times the font's line height, plus the
 * option count times max(0x18, 0x11), plus 0x1e, is the window's height. So a longer message
 * grows the box downwards and a longer option grows it sideways, and neither ever clips.
 *
 * WITH NO PARENT the window is style 0x442 and simply centred; with one it is style 0xc01 and
 * centred ON THE PARENT, by halving the difference of the two rectangles in each axis. The two
 * branches call DIFFERENT builders -- `build_window_with_port` takes eight arguments and
 * `GameWin_Init` nine -- so the parentless case is not the parented one with
 * a null passed in.
 *
 * The buttons are stacked, not side by side: a 0x3c x 0x19 rect is placed 0x46 in from the right
 * edge and 0x23 up from the bottom for OK, then moved 0x23 further up for Cancel. Both share one
 * handler and are told apart by their ids, 0x65 and 0x66.
 *
 * TWO SHIPPED BUGS, both about the copies the dialog makes of its strings. The allocation is
 * `new char[STRLEN(s + 1)]` and not `new char[STRLEN(s) + 1]` -- the `INC AX` lands on the
 * pointer's offset before the call, not on the length after it -- so the buffer is two bytes
 * short of what the following STRCPY writes, and an empty string sends STRLEN off the end. And
 * the buffers are released with `operator delete` (`__bdele_qnv_rtl`) rather than the array form,
 * which the matching `new[]` (`__bnwa_qui_rtl`) asked for.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

extern int MSGetLineHeight(unsigned h);
extern void build_window_with_port(GameWin *o, char *name, int style,
                                       int x, int y, int w, int h, void *bits);
extern void GameWin_Init(GameWin *o, char *name, int style, int x, int y, int w, int h, CTable *pal, Screen *parent);
extern void GRPort_FillColor_all(GRPort *p, int color);

extern void GRPort_Text(GRPort *p, unsigned *font, const char *s, RECT *r, int flags);
extern void EventWin_Modal(EventWin *o);
extern void set_dialog_result(void);
extern void end_dialog_with_value(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void game_font_set_1068_de7b(Font *font, int kind, int height);

extern "C" int font_handle_1068_dd1e(Font *font);

extern "C" void win_center_1068_dad3(DialogWin *o);
extern "C" void MSGetWinSize_of_1068_df03(void *o, RECT *r);
extern "C" void win_move_to_of(DialogWin *o, int x, int y);
extern "C" void FrameInfoRect(void *p, RECT *r, int depth, int light, int dark);
extern "C" void radio_init(SRadioSet *g, DialogWin *win, int id, RECT *r, int n, char *items);
extern "C" void set_control_action_1068_e03c(SRadioSet *g, void *h);
extern "C" void set_radio_dblclk_1068_e056(SRadioSet *g, void *h);
extern "C" void ctl_init_button_1068_daf8(SButton *button, DialogWin *win, int id, RECT *r, char *title);
extern "C" void set_button_action_1068_db5c(SButton *button, void *h);
extern "C" void show_window_1068_db76(void *o);

extern "C" int ask_choice(char *msg, char *options, void *parent)
{
    register int lines;
    register int lh;
    CTable ctable;
    Font font;
    GameWin win;
    SButton ok;
    SButton cancel;
    SRadioSet radios;
    int n;
    int w;
    int rowh;
    RECT box;
    RECT list;
    RECT text;
    RECT owner;
    char *msgcopy;
    char *optcopy;

    msgcopy = new char[STRLEN(msg + 1)];
    STRCPY(msgcopy, msg);
    optcopy = new char[STRLEN(options + 1)];
    STRCPY(optcopy, options);
    game_font_set_1068_de7b(&font, 0, 0x11);
    set_button_font_1068_dec9(&font);
    set_control_font_1068_dee6(&font);
    n = text_widest_line(optcopy, &w, &font);
    w += 0x20;
    lines = text_line_count(msgcopy, w + 0x46, &font);
    g_dialog_result = 0;
    rowh = max_int_1068_d81a(0x18, 0x11);
    lh = MSGetLineHeight(font_handle_1068_dd1e(&font));
    SetRect(&box, 0, 0, w + 0x5a, n * rowh + lines * lh + 0x1e);
    if (parent == 0) {
        build_window_with_port(&win, "", 0x442, box.left, box.top, box.right, box.bottom, &ctable);
        win_center_1068_dad3(&win);
    } else {
        GameWin_Init(&win, "", 0xc01, box.left, box.top, box.right,
                                         box.bottom, &ctable, (Screen *)parent);
        MSGetWinSize_of_1068_df03((DialogWin *)parent, &owner);
        win_move_to_of(&win,
            (rect_width_1068_d79c(&owner) - rect_width_1068_d79c(&box)) / 2 + owner.left,
            (rect_height_1068_d7bb(&owner) - rect_height_1068_d7bb(&box)) / 2 + owner.top);
    }
    GRPort_FillColor_all(&win, 7);
    FrameInfoRect(&win, &box, -3, 0xff, 0);
    g_dialog_owner = (char *)&win;
    TextColor(g_draw_color);
    SetRect(&text, 10, 10, port_width_1068_df51(&win) - 10, lines * lh + 10);
    GRPort_Text(&win, (unsigned *)&font, msgcopy, &text, 1);
    SetRect(&list, text.left, text.bottom + 10, text.right - 0x46, port_height_1068_df70(&win) - 10);
    radio_init(&radios, &win, 0x64, &list, 1, optcopy);
    set_control_action_1068_e03c(&radios, (void *)set_dialog_result);
    set_radio_dblclk_1068_e056(&radios, (void *)end_dialog_with_value);
    SetRect(&box, 0, 0, 0x3c, 0x19);
    OffsetRect(&box, port_width_1068_df51(&win) - 0x46, port_height_1068_df70(&win) - 0x23);
    ctl_init_button_1068_daf8(&ok, &win, 0x65, &box, "OK");
    set_button_action_1068_db5c(&ok, (void *)dialog_ok_cancel);
    OffsetRect(&box, 0, -0x23);
    ctl_init_button_1068_daf8(&cancel, &win, 0x66, &box, "Cancel");
    set_button_action_1068_db5c(&cancel, (void *)dialog_ok_cancel);
    show_window_1068_db76(&win);
    EventWin_Modal(&win);
    delete msgcopy;
    delete optcopy;
    return g_dialog_result;
}
