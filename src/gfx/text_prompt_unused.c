/* text_prompt_unused: candidate for 1068:d290 (442 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: text_prompt_unused
 * name-source: descriptive
 *
 * The start of a text-entry dialog, abandoned. Six objects are built on the stack -- a colour
 * table, a GameWin, a Font, two SButtons and an SEditBox -- the font is set up (kind 0, height
 * 0x11) and made current, and the "OK" (0x65) and "Cancel" (0x66) buttons are placed 0x46 in from
 * the window's right and 0x23 up from its bottom, one above the other, both handing their click
 * to `dialog_ok_cancel`. Then everything is destroyed and 0 returned: the window is
 * never built and the edit box never initialised. No call to it and no reference to its address
 * appears anywhere in the disassembly.
 *
 * The buttons' parent is `&win` upcast to DialogWin: the address of a local, so the +0x26 is
 * folded into the `LEA` with no null test.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

/* same object: 4-byte `push cs; call ` (rule 47) */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void game_font_set_1068_de7b(Font *font, int kind, int height);

extern "C" void ctl_init_button_1068_daf8(SButton *button, DialogWin *win, int id, RECT *r, char *title);
extern "C" void set_button_action_1068_db5c(SButton *button, void *h);

int text_prompt_unused(void)
{
    CTable ct;
    GameWin win;
    Font font;
    SButton ok;
    SButton cancel;
    SEditBox edit;
    RECT r;

    game_font_set_1068_de7b(&font, 0, 0x11);
    set_button_font_1068_dec9(&font);
    set_editbox_font_1068_e13a(&font);
    SetRect(&r, 0, 0, 0x3c, 0x19);
    OffsetRect(&r, port_width_1068_df51(&win) - 0x46, port_height_1068_df70(&win) - 0x23);
    ctl_init_button_1068_daf8(&ok, &win, 0x65, &r, "OK");
    set_button_action_1068_db5c(&ok, (void *)dialog_ok_cancel);
    OffsetRect(&r, 0, -0x23);
    ctl_init_button_1068_daf8(&cancel, &win, 0x66, &r, "Cancel");
    set_button_action_1068_db5c(&cancel, (void *)dialog_ok_cancel);
    return 0;
}

