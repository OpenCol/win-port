/* AboutWin_ctor: candidate for 1060:72c8 (1168 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: AboutWin::AboutWin()
 *
 * THE ABOUT BOX'S CONSTRUCTOR, and with it the class: an EventWin, a colour table, three ports, a packed
 * palette, three rectangles, a sprite, 21 frame sprites, the cursor sprite and 25 counters -- 0xbd6 bytes.
 *
 * It builds a FULL-SCREEN window first, black and centred, and shows it; that is the backdrop. Picture
 * 0x2ee goes into the first port, and picture 0xcb is cut into the cursor sprite with 0x78 transparent.
 * The top 100 rows of the picture go into the second port and the bottom 100 into the third -- the two
 * strips the About box scrolls between. The box's own window is then built over the backdrop at the
 * picture's size, centred, and the picture copied into it.
 *
 * Both windows get the same two handlers, `AboutWin_Mouse` on slot 4 and `AboutWin_Keys`
 * on slot 32, on the backdrop through +0xd1 and on the box through +0x33.
 *
 * Finally the 25 counters are zeroed, DGROUP:0x4e66 is cleared, the letter sprites are built and
 * DGROUP:0x4e64 is cleared.
 *
 * Everything before the body is the compiler's: the `this == 0` allocation of 0xbd6 bytes, the base and
 * member constructors, and the 21-element array through VNEWVF.
 *
 * THE EXCEPTION DESCRIPTOR COUNTS SUB-OBJECTS. The word before this function (Borland's
 * per-function unwind table, 14 bytes at 1060:72ba) has an entry for the destructible local, and its
 * third word is the number of destructor slots that PRECEDE that local in the unwind numbering:
 * for a constructor, every base and member of `this` with a destructor, counted transitively
 * through the bases' own bases and members, array elements one each (established with BCC 4.52
 * probes, 2026-09-28). The shipped word is 0x24 = 36: GameWin's five (GRPort, DialogWin, EventWin, Screen, EventTable),
 * `win`'s three (itself, Screen, EventTable), `table`, `p1`..`p3`, `s1`, 21 frames and `cursor`; with `GameWin` and `EventWin` declared as opaque
 * blobs this file produced 0x1d = 29, so the classes are declared with their real chain -- GRPort +
 * DialogWin(EventWin(Screen, EventTable)) -- which is what the rest of the corpus declares
 * (ColChild_ctor_1048_e93b, SuperWin_ctor_1060_6068). The code bytes do not change; the
 * grader masks the table, only tools/check_data_refs.py compares it.
 */

#include "colonization.h"

#include "fathers.h"
#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

extern void MSHideMenuBar(void);

struct logpal;
extern void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);
extern void EventWin_Init(char *w, char *name, int style, int x, int y,
                                       int width, int height, CTable *pal);
extern void GameWin_Init(GameWin *o, char *name, int style, int x, int y, int w, int h, CTable *pal, Screen *parent);
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);
extern void Sprite_Extract_key_rect(Sprite *sprite, void *src, unsigned char transparent, RECT *r);
extern int GRPort_Init(GRPort *p, void *r);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

extern void init_letter_sprites(AboutWin *o);
extern void AboutWin_Mouse(void);
extern void AboutWin_Keys(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void win_set_back_color_1060_85da(EventWin *o, int index);
extern "C" void win_center_1060_85fe(void *o);
extern "C" void MSShowWindow_of_1060_8623(void *w);
extern "C" void *set_on_lbutton_down_1060_8644(void *table, void *h);
extern "C" void *set_on_keydown_1060_867d(void *table, void *h);

extern "C" int port_width_1060_86c9(void *port);
extern "C" int port_height_1060_86e2(void *port);

AboutWin::AboutWin()
{
    GRPort tmp;
    RECT desk;
    int i;

    MSHideMenuBar();
    MSGetDesktopSize(&desk);
    CTable_CopyRawColors((CTable *)&g_game_palette, palette, 10, 0xec);
    EventWin_Init((char *)&win, "", 0x801, 0, 0, desk.right, desk.bottom, &g_game_palette);
    win_set_back_color_1060_85da((DialogWin *)&win, 0);
    win_center_1060_85fe((DialogWin *)&win);
    MSShowWindow_of_1060_8623((DialogWin *)&win);
    set_on_lbutton_down_1060_8644(&(EventTable &)win, AboutWin_Mouse);
    set_on_keydown_1060_867d(&(EventTable &)win, AboutWin_Keys);
    MSDoAllTasks_1060_86b6();
    GRPort_LoadPicture(&pic, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
    GRPort_LoadPicture(&tmp, 0xcb, 0xd3, 0x18, (struct logpal *)&g_game_palette);
    SetRect(&desk, 0, 0, port_width_1060_86c9(&tmp), port_height_1060_86e2(&tmp));
    Sprite_Extract_key_rect(&tile, &tmp, 0x78, &desk);
    SetRect(&top_strip, 0, 0, port_width_1060_86c9(&pic), 0x64);
    GRPort_Init(&page, &top_strip);
    GRPort_CopyToPort(&pic, &page, &top_strip, &top_strip);
    SetRect(&bottom_strip, 0, port_height_1060_86e2(&pic) - 0x64, port_width_1060_86c9(&pic),
            port_height_1060_86e2(&pic));
    GRPort_Init(&overlay, &top_strip);
    GRPort_CopyToPort(&pic, &overlay, &bottom_strip, &top_strip);
    SetRect(&full, 0, 0, port_width_1060_86c9(&pic), port_height_1060_86e2(&pic));
    GameWin_Init(this, "", 0x801, 0, 0, port_width_1060_86c9(&pic),
                                     port_height_1060_86e2(&pic), &g_game_palette, &win);
    win_center_1060_85fe(&(DialogWin &)*this);
    set_on_lbutton_down_1060_8644(&(EventTable &)*this, AboutWin_Mouse);
    set_on_keydown_1060_867d(&(EventTable &)*this, AboutWin_Keys);
    GRPort_CopyToPort(&pic, this, &full, &full);
    for (i = 0; i < FATHER_COUNT; i++)
        shown[i] = 0;
    g_about_step = 0;
    init_letter_sprites(this);
    g_about_done = 0;
}
