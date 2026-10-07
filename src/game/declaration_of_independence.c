/* declaration_of_independence: candidate for 1038:660e (1183 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: declaration_of_independence
 *
 * SPELLS OUT THE NATION'S NAME in animated letters, from `..\SOURCE\GAME\LIBERTY.CPP` -- the four
 * asserts name the file and give the source's own `i` and `j`. `1038:6aad` is the only caller.
 *
 * The text is the record at SEG20:0x7810 for the nation at SEG20:0x779a. Each letter takes TEN sprites,
 * ten frames of one letter's animation: capitals start at resource 0x1388 and lower case at 0x1770, ten
 * resources apart per letter. They are loaded into one `new Sprite[800]` -- 80 letters' worth, which is
 * what the asserts guard.
 *
 * A letter that would pass x = 500 is skipped, and once the line passes x = 512 the text is CUT THERE:
 * ten frames of sprite 0x1b58 are loaded as the last glyph and the string is truncated in place.
 *
 * The animation then plays: for each letter, its ten frames are drawn in turn at (x, y) with a tick
 * between them and only that rectangle copied to the screen, the line creeping four pixels UP per letter
 * and across by the width of the letter's last frame. The dialog then runs modally.
 *
 * TWO QUIRKS: the array is released with a scalar `delete`, so only the first sprite's destructor runs;
 * and the assert macro is `if (e) ; else assert_fail(...)`, which is what puts the empty then-arm's jump
 * in front of every one of them.
 *
 * RENAMED 2026-09-27 from liberty_spell_name: Mac declaration_of_independence(void) references five assert strings: 'LIBERTY.CPP:1010 (i*10) + j < 800', 1014 '(i*10)+ 9 < 800', 1024, 1046 and 1052. 660e is the Windows function holding the same assert expressions from ..\SOURCE\GAME\LIBERTY.CPP ('(i*10) + j < 800', '(i*10)+ 9 < 800', '(i*10)+ j < 800'). No other function has them on either side. Call graph: the Mac function's only caller is revolution, and 660e's only caller is 6aad, which becomes revolution. It is at the same position in the sequence, after force_mood and before sort_16x16. The bodies match: Sprite ctor/Init, SuperWin::Init, MSCenterWindow, MSShowWindow, MSWaitTicks, Sprite::Draw, SuperWin::Execute and GoAway on Mac, against Sprite_Init x3, SuperWin_Init, win_center, show_window, MSWaitTicks, Sprite_Draw, SuperWin_Execute and SuperWin_dtor on Windows. This is the animated signing screen. Arity: (void) on both sides. Size is 2246 Mac against 1183 Windows. The current name is descriptive: its own notes say it is 'deliberately about provenance, not behaviour', and its 'high' grade is for the purpose, not for a developers' name. The file is C++ (extern "C" block). Apply this only after 1038:6aad has moved to revolution.
 *
 * THE EXCEPTION DESCRIPTOR COUNTS SUB-OBJECTS. Borland's per-function unwind table before this
 * function (1038:65f0) has three entries; the third, for the `new Sprite[800]` under construction,
 * carries in its third word the number of destructor slots that PRECEDE it in the unwind
 * numbering: `dlg` itself plus every base and member of `dlg` with a destructor, counted
 * transitively, plus the ten `letters` (established with BCC 4.52 probes, 2026-09-28). Shipped
 * 0x14 = 20 = 1 + 9 + 10; with `SuperWin` an opaque 0x414-byte blob this file produced
 * 0x0b = 11, so SuperWin is declared with its real chain -- GameWin (GRPort + DialogWin over
 * EventWin(Screen, EventTable)) and its own EventWin member -- as SuperWin_ctor_1060_6068
 * declares it. The code bytes do not change; the grader masks the table, only
 * tools/check_data_refs.py compares it.
 *
 * THE DIALOG CLASS IS `SuperWin`, renamed from `EventDialog` 2026-09-28: its destructor call lands on
 * SuperWin::~SuperWin (1050:6b7d), and the image's RTTI names `SuperWin` (never `EventDialog`)
 * in this module's segment. Its constructor is the module's inline copy (EventDialog_ctor_*).
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */
#pragma option -a
class Sprite { public: RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; short handle; unsigned char *bits; Sprite(); ~Sprite(); };
#pragma option -a-
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DlgControl; class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class SuperWin : public GameWin {
public:
    EventWin win;
    char palette[0x414 - 0x150];
    SuperWin();
    ~SuperWin();
};
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

/* Borland's character-class table (CWL.LIB's ctype), indexed as ctype.h's macros do:
   `_ctype[(c) + 1] & _IS_DIG` (2), _IS_UPP (4), _IS_LOW (8). Declared : the shipped code
   reaches it through its segment, and BCC refuses a redeclaration after ctype.h */
extern unsigned char _ctype[257];   /* DGROUP:0x46c0, declared */

extern void Sprite_Init(Sprite *s, int res);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void SuperWin_Init(SuperWin *w);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);
extern void SuperWin_Execute(SuperWin *w);
extern void SuperWin_GoAway(SuperWin *w);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void win_center_1038_8837(void *w);
extern "C" void show_window_1038_885c(SuperWin *w);

extern "C" void copy_rect_to_screen_1038_888e(SuperWin *w, RECT *r);

void declaration_of_independence(void)
{
    SuperWin dlg;
    Sprite letters[10];
    RECT r;
    char *p;
    int x;
    int y;
    Sprite *sprites;
    int base;
    int off;
    int count;
    int step;
    int space;
    char buf[0x50];
    register int j;
    register int i;

    sprites = new Sprite[0x320];
    if (sprites == 0)
        return;
    STRCPY(buf, (char *)&g_nation_text[g_game.player_nation]);
    step = 4;
    p = buf;
    Sprite_Init(sprites, 0x1b61);
    space = rect_width_1038_87fa(sprites);
    i = 0;
    x = 0x104;
    while (*p != 0) {
        if ((_ctype[*p + 1] & 0xc) && x + space < 0x1f4) {
            if (_ctype[*p + 1] & 4) {
                base = 0x1388;
                off = *p - 0x41;
            } else if (_ctype[*p + 1] & 8) {
                base = 0x1770;
                off = *p - 0x61;
            }
            off = off * 10;
            for (j = 0; j < 10; j++) {
                if ((i * 10) + j < 800)
                    ;
                else
                    assert_fail("Assertion failed: %s, file %s, line %d", "(i*10) + j < 800",
                                "..\\SOURCE\\GAME\\LIBERTY.CPP", 1010);
                Sprite_Init(&sprites[(i * 10) + j], base + off + j);
            }
            if ((i * 10) + 9 < 800)
                ;
            else
                assert_fail("Assertion failed: %s, file %s, line %d", "(i*10)+ 9 < 800",
                            "..\\SOURCE\\GAME\\LIBERTY.CPP", 1014);
            x += rect_width_1038_87fa(&sprites[(i * 10) + 9]);
        }
        i++;
        p++;
        if (x + space >= 0x200) {
            base = 0x1b58;
            for (j = 0; j < 10; j++) {
                if ((i * 10) + j < 800)
                    ;
                else
                    assert_fail("Assertion failed: %s, file %s, line %d", "(i*10)+ j < 800",
                                "..\\SOURCE\\GAME\\LIBERTY.CPP", 1024);
                Sprite_Init(&sprites[(i * 10) + j], base + j);
            }
            i++;
            *p = 0;
            break;
        }
    }
    count = i;
    x = 0x104;
    y = 0x159;
    SuperWin_Init(&dlg);
    GRPort_LoadPicture(&dlg, 0x136, 10, 0xdc, (struct logpal *)&g_game_palette);
    copy_all_to_screen_1038_8819(&dlg);
    win_center_1038_8837(&(DialogWin &)dlg);
    show_window_1038_885c(&dlg);
    MSDoAllTasks_1038_887b();
    for (i = 0; i < count; i++) {
        for (j = 0; j < 10; j++) {
            if ((i * 10) + j < 800)
                ;
            else
                assert_fail("Assertion failed: %s, file %s, line %d", "(i*10)+ j < 800",
                            "..\\SOURCE\\GAME\\LIBERTY.CPP", 1046);
            Sprite_Draw(&r, &sprites[(i * 10) + j], &dlg, x, y);
            copy_rect_to_screen_1038_888e(&dlg, &r);
            MSWaitTicks_1038_8927(1);
        }
        y -= step;
        if ((i * 10) + 9 < 800)
            ;
        else
            assert_fail("Assertion failed: %s, file %s, line %d", "(i*10)+ 9 < 800",
                        "..\\SOURCE\\GAME\\LIBERTY.CPP", 1052);
        x += rect_width_1038_87fa(&sprites[(i * 10) + 9]);
    }
    SuperWin_Execute(&dlg);
    SuperWin_GoAway(&dlg);
    delete sprites;
}

