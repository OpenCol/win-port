/* destroy_game_globals: candidate for 1008:efbd (446 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: _STDES_()
 * name-source: descriptive
 *
 * Module 1008's static destructor: every file-scope object `create_game_globals`
 * builds, torn down in exactly the reverse order -- the screen port, the nine fonts from
 * DGROUP:0x4b26 down, the CursorClass and Sprite tables through `VDELF` with the element
 * destructor's address, the lone Sprite at SEG20:0x7526 directly, then the debug window,
 * the main window, the palette and the preferences. Each destructor is handed flag word 2:
 * destroy, do not free.
 *
 * THE CLASSES ARE THE REAL ONES (2026-09-28): see create_game_globals.c -- the module's type
 * descriptors for MainWin, DebugWin and GRPort need every window declared with its real bases
 * and members.
 *
 * NOTHING IN THE SOURCE IS THIS FUNCTION (§175). This file is the constructor's text with the
 * symbol changed; Borland writes both halves from the same definitions. It carries no exception
 * block. The exception table after it is where the array types are spelled out --
 * "CursorClass[15]", "Sprite[16]", "Sprite[1]", "Sprite[18]", "Sprite[132]", "Sprite[44]",
 * "Sprite[190]", "Sprite[27]" -- which is how the array lengths below and the name
 * `CursorClass_ctor` are known.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
class SButton;
#include "records.h"
template <class T> class LinkedList : public VList {
public:
    LinkedList();
};
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
class MainWin : public EventWin {
public:
    MenuClass menu;
    MapWin map;
    MiniWin mini;
    StatWin stat;
    ReportWin report;
    ColonyWin colony;
    EuropeWin europe;
    PediaWin pedia;
    MainWin();
    ~MainWin();
};

/* same object: 4-byte `push cs; call ` (rule 47), from here or from the _STDES_ */

/* PORT: an in-file stub of Font::Font removed: src/game/Font_ctor_1008_d478.c defines it (W2). */

/* PORT: an in-file stub of Font::~Font removed: src/game/Font_dtor_1008_d4b5.c defines it (W2). */

GamePrefs g_prefs_root;            /* SEG20:0x10 */
CTable g_game_palette;             /* SEG20:0x1ac */
MainWin g_main_win;             /* SEG20:0x5ca */
/* MainWin is 0x3b0c bytes (its shipped type descriptor, and MainWin_ctor's member offsets), so
 * 0xc bytes separate it from the debug window: g_game_brains, the GameBrains object every
 * function of module 1008:2927..3cc8 runs on. It has no constructor (nothing is built between
 * the two windows), and is spelled as an anonymous block like the gap below. */
static char gap_40d6[0x40e2 - 0x40d6];
DebugWin g_debug_win;              /* SEG20:0x40e2 */
Sprite g_art_base[0x1b];   /* SEG20:0x41a6, the first art sprites. Its readers once
   indexed it past its end ([411], [412] for the background tile and the route panel); the
   shipped fixups name those sprites' own symbols, so they read g_background_tile and
   g_route_panel_sprite now, and every remaining reference is to this array (2026-10-02). */
Sprite g_terrain_sprites[0xbe];       /* SEG20:0x4506 */
Sprite g_overlay_sprites[0x2c];       /* SEG20:0x5cc6 */
Sprite g_icon_sprites[0x84];       /* SEG20:0x6246 */
Sprite g_flag_art[0x12];       /* SEG20:0x72c6 */
/* PORT: one Sprite, as every user of g_background_tile takes it; the shipped data held an array of one (W1). */
Sprite g_background_tile;            /* SEG20:0x7506 */
Sprite g_route_panel_sprite;              /* SEG20:0x7526 */
Sprite g_edge_sprites[0x10];       /* SEG20:0x7546 */
CursorClass g_cursors[0xf];        /* SEG20:0x7746 */
Font g_title_font;                     /* DGROUP:0x4b06 */
Font g_report_font_small;              /* DGROUP:0x4b0a */
Font g_font_tiny;                      /* DGROUP:0x4b0e */
Font g_info_font_small;                /* DGROUP:0x4b12 */
Font g_info_font_large;                /* DGROUP:0x4b16 */
Font g_number_font;                    /* DGROUP:0x4b1a */
Font g_font_small;                     /* DGROUP:0x4b1e */
Font g_heading_font;                   /* DGROUP:0x4b22 */
Font g_report_font;                    /* DGROUP:0x4b26 */
/* THE GAP BEFORE THE SCREEN PORT. This TU's segment is laid out in definition order, so
 * g_cursors ending at 0x7782 would put g_sprite_sheet there; it ships at 0xdea0. The 0x671e
 * bytes between are the rest of the original TU's definitions -- the game state from
 * g_game.tutorial_flags (0x7782) through the colony, unit, nation and tribe tables to
 * g_terrain_colour (0xde82), about 110 globals the files that use them declare by address.
 * None has a constructor (this function builds nothing between the cursors and the port), and
 * two runs are INITIALISED in the shipped segment: the eight tribe colours at 0xd9ce
 * (g_tribe_colour) and 0x28 bytes at 0xdc98 (a 16-entry permutation, then the 4-byte rows of
 * the building-needs table at 0xdca8). They are spelled here as anonymous `static` blocks so
 * the layout and the initialised bytes are the original's without this file claiming names or
 * types for them. Declaring g_sprite_sheet `extern` instead is not an option: it drops the
 * port's construction at +0x1a1 (and its destruction in destroy_game_globals), and grades
 * DIFFERS. */
static char gap_7782[0xd9ce - 0x7782];
static unsigned char init_d9ce[8] = { 0x0f, 0x95, 0x36, 0x0b, 0x43, 0x6f, 0x75, 0x47 };
static char gap_d9d6[0xdc98 - 0xd9d6];
static unsigned char init_dc98[0x28] = {
    0x07, 0x0b, 0x09, 0x0a, 0x0c, 0x01, 0x04, 0x02, 0x03, 0x06, 0x05, 0x0d, 0x08, 0x0e, 0x0f, 0x00,
    0x03, 0x0f, 0x0e, 0x0f, 0x27, 0x0e, 0x06, 0x0e, 0x20, 0x0c, 0x04, 0x0c,
    0x1b, 0x09, 0x01, 0x09, 0x18, 0x0a, 0x02, 0x0a, 0x15, 0x0b, 0x03, 0x0b };
static char gap_dcc0[0xdea0 - 0xdcc0];
GRPort g_sprite_sheet;           /* SEG20:0xdea0 */
