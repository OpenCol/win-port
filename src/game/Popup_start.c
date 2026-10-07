/* PORT: this file's own declarations of Popup_check, Popup_edit, win_disable_1050_8c93 removed: the header declares them from the definition (W1 step 3). */
/* Popup_start: candidate for 1050:54f1 (1807 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 1807, not the census 1798. The last nine bytes are a dead second epilogue.
 *
 * THE WHOLE DIALOG-DESCRIPTION LANGUAGE, in one loop. A TEXT resource is opened by name, locked, and
 * read a line at a time; a line starting with `@` is a directive and anything else is content. This is
 * what turns a text file into a laid-out popup, and the fifteen keywords it understands are the entire
 * grammar:
 *
 *   @OPTIONS and @PROMPT   switch to content mode 2, where every following line becomes a list item;
 *   @TEXT                  switches back to mode 1, where lines are appended as body text;
 *   @TITLE=, @BUTTON=      take the text after the `=` and set the title or add a button;
 *   @COLUMNS, @HEIGHT,
 *   @WIDTH, @X, @Y         take the first number that follows and set a dimension;
 *   @LENGTH                sets the selected item, but only if the caller did not pass one;
 *   @SMALLFONT             switches the font;
 *   @CHECKBOX              sets bits 0 and 2 of +0x86 together;
 *   @LISTBOX               turns the item list into a sixteen-wide list box;
 *   @DEFAULT               is the odd one: in edit mode it is the initial text of the edit field, and
 *                          otherwise it is a selected-item number like @LENGTH.
 * Anything else ENDS THE PARSE -- an unknown directive sets the state to 3 and the loop stops, so a
 * typo in a keyword silently truncates the dialog at that line.
 *
 * TWO KINDS OF SKIP follow a keyword, and which one is used says what the value is. A text directive
 * scans to the `=` and steps one past it; a numeric one scans forward until the character class table
 * at DGROUP:0x46c1 says DIGIT, so `@width 40` and `@width=40` and `@width is 40` all work.
 *
 * ONE KEYWORD IS CASE-SENSITIVE. Every other comparison is `STRICMP` or `STRNICMP`, but PROMPT goes
 * through `STRCMP` -- so `@prompt` is not recognised while `@options` is, and the two are otherwise
 * the same directive. The table in DGROUP is upper case and the resources are written lower, which is
 * why nobody noticed.
 *
 * AN EMPTY LINE ADVANCES THE STATE, so the resource is read as: body text, blank line, items, blank
 * line, end. That is also the only way the loop terminates normally.
 *
 * WHAT THE BYTES SETTLED: both character comparisons carry an explicit `(int)`. Without it Borland
 * folds `*p == '@'` into a one-instruction `CMP BYTE [BX],0x40` where the shipped loads, sign-extends
 * and compares a word (rule 59's family). The digit test keeps its `!`, which widens, so the two
 * spellings sit side by side in the same function.
 *
 * `g_name_slots` (SEG31:0xa4) is the dialog's TEXT-ENTRY buffers: `char[][0x50]`, one slot
 * per edit control, which this function indexes with the popup's edit-field count `p->n_edits` (the next field's slot)
 * and `Popup_exec` fills with `ctl_get_text(&p->edits[i], g_name_slots[i])`.
 *
 * SIX NAMES FOR ONE ARRAY, AND NOW ONE. The single-field popups reached slot 0 under their
 * own names -- `g_name_buf` as text, `g_text_answer` as a number that gets `ATOL_1050_8de2`'d,
 * `g_msg_name` in `ColonyWin_colony_hammer_buy`, `g_default_route_name` in `route_create` -- and the
 * popup machinery called the whole array `g_edit_bufs`. `check_global_names.py` tolerated it
 * on the grounds that collapsing it meant changing the expressions at the use sites, which
 * was a byte risk for a cosmetic gain.
 *
 * It was not a risk: `&g_name_buf` and `g_name_slots[0]` are the same address and compile
 * to the same push, and all fifteen files still grade EXACT or RELOCATABLE. Nor was the gain
 * cosmetic -- a behaviour-exact port emits one object per NAME, so six of them would have got
 * six separate buffers and writes through one would have stopped being visible through the
 * others. The name that won is the one sixteen files already used.
 *
 * RENAMED 2026-09-27 from text_directive_parser: CONST PROPOSAL. Mac Popup::start(char*, char*, int, char*, GameWin*, GRPort*, Rect*, int), CODE 19 0x58e0. 54f1 (Popup *popup, void *unused, char *name, int sel, char *dflt, int a18, int a1a, int a1c, int a1e, int rect_off, int rect_seg, int kind) is this plus: char*, char*, int, char*, then the GameWin* and GRPort* word-split into four ints, then Rect* split into two, then int. That is all eight Mac parameters. Callees: resource get/lock/unlock/release, Popup::init (= popup_setup), text_get x2 (= read_line x2), stricmp/strnicmp (x3/x11), popup_string (= expand_text_template, already Mac-bound), set_title, button, set_columns, set_height, set_font, set_width, set_list, string, edit, check, item. 54f1's list is the same. Callers include popup_ask and popup_ask_nocancel on both sides. Layout: Mac exec, text_get, start; Windows Popup_exec, read_line, 54f1. Size 1807. Update tools/check_split_args.py EXEMPT, which is keyed by the name 'text_directive_parser'.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

#include "classes.h"

/* Borland's character-class table (CWL.LIB's ctype), indexed as ctype.h's macros do:
   `_ctype[(c) + 1] & _IS_DIG` (2), _IS_UPP (4), _IS_LOW (8). Declared : the shipped code
   reaches it through its segment, and BCC refuses a redeclaration after ctype.h */
extern unsigned char _ctype[257];   /* DGROUP:0x46c0, declared */

/* 5-byte NOP form, so extern (rule 47) */
extern void expand_text_template(char *src, char *dst);

/* same object: 4-byte `push cs; call ` (rule 47) */
#include "w16types.h"

extern void *MSLockResource_1050_8dc2(void *h);
extern void MSUnlockResource_1050_8e02(void *h);
extern void MSReleaseResource_1050_8e1e(void *h);
extern void Popup_init(Popup *popup, GameWin *win, GRPort *port, RECT *r, int kind);

extern void Popup_set_title(Popup *popup, char *s);
extern void Popup_button(Popup *popup, char *s);
extern int ATOL_1050_8de2(char *digits);
extern void Popup_set_columns(Popup *popup, int columns);
extern void Popup_set_height(Popup *popup, int height);
extern void Popup_set_width(Popup *popup, int width);
extern void Popup_set_font(Popup *popup, Font *new_font);
extern void Popup_set_list(Popup *popup, int a, int b);
extern void Popup_string(Popup *popup, char *s);
extern void *Popup_item(Popup *popup, char *s, int id);

int Popup_start(Popup *popup, void *unused, char *name, int sel,
                              char *dflt, GameWin *win, GRPort *port,
                              RECT *rect, int kind)
{
    register int idx;
    register int state;
    int ret;
    int id;
    int num;
    int edit;
    char *line;
    char *q;
    char *dp;
    void *item;
    void *ctx;
    void *h;
    char *data;
    char buf[0x100];
    char dbuf[0x50];
    char rname[0x20];

    idx = sel;
    ret = 0;
    state = 1;
    id = 0;
    ctx = g_restype_text_popup;
    win_disable_1050_8c93((DialogWin *)g_popup_owner);
    STRCPY(rname, name);
    h = resource_find_1050_8d9c((long *)&ctx, rname);
    if (h == 0)
        return 1;
    data = (char *)MSLockResource_1050_8dc2(h);
    if (data == 0)
        return 1;
    Popup_init(popup, win, port, rect, kind);
    edit = dflt != 0 ? 1 : 0;
    if (edit != 0)
        STRCPY(dbuf, dflt);
    read_line(0);
    do {
        line = read_line(data);
        if (STRLEN(line) == 0) {
            state++;
            continue;
        }
        if ((int)*line == '@') {
            q = line + 1;
            if (STRICMP(q, "OPTIONS") == 0 || STRCMP(q, "PROMPT") == 0)
                state = 2;
            else if (STRICMP(q, "TEXT") == 0)
                state = 1;
            else if (STRNICMP(q, "TITLE", 5) == 0) {
                while (*q != 0 && (int)*q != '=')
                    q++;
                if (*q != 0)
                    q++;
                expand_text_template(q, buf);
                Popup_set_title(popup, buf);
            } else if (STRNICMP(q, "BUTTON", 6) == 0) {
                while (*q != 0 && (int)*q != '=')
                    q++;
                if (*q != 0)
                    q++;
                expand_text_template(q, buf);
                Popup_button(popup, buf);
            } else if (STRNICMP(q, "COLUMNS", 7) == 0) {
                while (*q != 0 && !(_ctype[*q + 1] & 2))
                    q++;
                num = ATOL_1050_8de2(q);
                Popup_set_columns(popup, num);
            } else if (STRNICMP(q, "HEIGHT", 6) == 0) {
                while (*q != 0 && !(_ctype[*q + 1] & 2))
                    q++;
                num = ATOL_1050_8de2(q);
                Popup_set_height(popup, num);
            } else if (STRICMP(q, "SMALLFONT") == 0) {
                while (*q != 0 && !(_ctype[*q + 1] & 2))
                    q++;
                Popup_set_font(popup, g_popup_font_small);
            } else if (STRNICMP(q, "Y", 1) == 0) {
                while (*q != 0 && !(_ctype[*q + 1] & 2))
                    q++;
                popup->req_y = ATOL_1050_8de2(q);
            } else if (STRNICMP(q, "X", 1) == 0) {
                while (*q != 0 && !(_ctype[*q + 1] & 2))
                    q++;
                popup->req_x = ATOL_1050_8de2(q);
            } else if (STRNICMP(q, "WIDTH", 5) == 0) {
                while (*q != 0 && !(_ctype[*q + 1] & 2))
                    q++;
                num = ATOL_1050_8de2(q);
                Popup_set_width(popup, num);
            } else if (STRNICMP(q, "LENGTH", 6) == 0) {
                while (*q != 0 && !(_ctype[*q + 1] & 2))
                    q++;
                if (idx == 0)
                    idx = ATOL_1050_8de2(q);
            } else if (STRNICMP(q, "CHECKBOX", 7) == 0)
                popup->flags |= 5;
            else if (STRNICMP(q, "LISTBOX", 7) == 0)
                Popup_set_list(popup, 0x10, popup->width);
            else if (STRNICMP(q, "DEFAULT", 7) == 0) {
                if (edit != 0) {
                    while (*q != 0 && (int)*q != '=')
                        q++;
                    if (*q != 0)
                        q++;
                    if (STRLEN(dflt) == 0)
                        STRCPY(dbuf, q);
                } else {
                    while (*q != 0 && !(_ctype[*q + 1] & 2))
                        q++;
                    if (idx == 0)
                        idx = ATOL_1050_8de2(q);
                }
            } else
                state = 3;
        } else {
            switch (state) {
            case 1:
                expand_text_template(line, buf);
                Popup_string(popup, buf);
                break;
            case 2:
                expand_text_template(line, buf);
                if (edit != 0) {
                    if (idx == 0)
                        idx = 5;
                    dp = dbuf;
                    if (popup->n_edits != 0)
                        dp = g_name_slots[popup->n_edits];
                    Popup_edit(popup, buf, dp, idx);
                } else {
                    if (popup->flags & 4)
                        item = Popup_check(popup, buf, id, 0);
                    else
                        item = Popup_item(popup, buf, id);
                    if (id == idx)
                        popup->sel_item = (struct PopupItem *)item;
                    id++;
                }
                break;
            }
        }
    } while (state < 3);
    ret = 0;
    MSUnlockResource_1050_8e02(h);
    MSReleaseResource_1050_8e1e(h);
    return ret;
}
