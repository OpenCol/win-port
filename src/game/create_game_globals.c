/* create_game_globals: candidate for 1008:ee00 (445 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: _STCON_()
 * name-source: descriptive
 * objects-before: OpenBackdrop
 *
 * Module 1008's static constructor, and the largest in the image: it builds the game's
 * file-scope objects in definition order -- the preferences at SEG20:0x10, the game palette at
 * 0x1ac, the main window at 0x5ca and the debug window at 0x40e2; seven sprite tables from
 * 0x41a6 to 0x7746, 0x1b, 0xbe, 0x2c, 0x84, 0x12, 1 and 0x10 sprites long with one lone Sprite
 * between the last two; fifteen CursorClass objects at 0x7746; the nine fonts at DGROUP:0x4b06..0x4b26; and
 * the screen port at SEG20:0xdea0. `destroy_game_globals` is the matching `_STDES_`,
 * compiled from this same text.
 *
 * THE CLASSES ARE THE REAL ONES (2026-09-28): the module's type descriptors for MainWin (0x3b0c
 * bytes, 0x8d destructible sub-objects: itself, EventWin's three, the MenuClass and the seven
 * child windows' own counts), DebugWin (a GameWin, vptr at +0) and GRPort (polymorphic) are
 * only reproduced by declaring every window with its real bases and members, as their own
 * constructors do; opaque blobs gave descriptors that exist nowhere in the image.
 *
 * NOTHING IN THE SOURCE IS THIS FUNCTION (§175): it is the definitions below. The arrays go
 * through `VNEWVF` with the element constructor's address, the single objects through a
 * direct call; `` on a definition is `PUSH SEG` and its absence `PUSH DS`, which is how the
 * fonts are known to live in the data group. The sprite table at 0x7506 is an array of ONE
 * Sprite -- the next object, at 0x7526, is a plain Sprite and is constructed directly -- so the
 * source said `[1]` for one and not the other.
 *
 * THIS FILE IS WHERE THE NINE FONT NAMES COME FROM, and it is worth saying what they do
 * NOT rest on. The constructor default-builds each Font in definition order and loads no
 * resource, so nothing here says which typeface a slot ends up holding. Before this the
 * nine slots carried four to seven names each, every file naming the slot after whatever
 * it happened to draw with -- and they were CROSS-WIRED: `g_report_font` meant 0x4b1e in
 * three files and 0x4b26 in six, `g_row_font` meant 0x4b1a and 0x4b22. One name per slot,
 * taken from the definitions below, is what the corpus now uses. The words are a
 * reasonable guess; the slot each belongs to is not a guess.
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
/* THE NEAR GAME STATE AFTER THE FONTS. This TU also DEFINES the DGROUP block from 0x4b2a on, and
 * the bytes say so: load_game_file and save_game take these addresses as `push ds; push off`,
 * which Borland emits only for a symbol this TU defines (or never declared ) -- while
 * orders_build, check_tut, end_of_game_check and the GameBrains functions reach the same
 * globals through `` redeclarations (es:). A extern followed by this definition
 * reproduces both. The run is laid out in definition order, so every slot up to the last
 * address taken (g_nation_strength, 0x4b6c) is defined. The byte at 0x4b2d has no reader and
 * stays anonymous; the three word arrays are sized from the gaps to the next known global. */
int g_4b2a;                            /* DGROUP:0x4b2a */
char g_panel_shown_mask;               /* DGROUP:0x4b2c */
static char pad_4b2d;                  /* DGROUP:0x4b2d, no reader */
int g_seam_handle;                     /* DGROUP:0x4b2e */
int g_cursor_x;                        /* DGROUP:0x4b30 */
int g_cursor_y;                        /* DGROUP:0x4b32 */
int g_season_word[2];                  /* DGROUP:0x4b34 */
int g_cmisc_word[3];                   /* DGROUP:0x4b38 */
int g_eurolabel_word[3];               /* DGROUP:0x4b3e */
unsigned char g_nation_unit_count[4];      /* DGROUP:0x4b44 */
unsigned char g_nation_colony_count[4];    /* DGROUP:0x4b48 */
unsigned char g_nation_free_colonists[4];  /* DGROUP:0x4b4c */
unsigned char g_pop_by_nation[4];          /* DGROUP:0x4b50 */
unsigned char g_nation_population[4];      /* DGROUP:0x4b54 */
unsigned char g_nation_land_strength[4];   /* DGROUP:0x4b58 */
unsigned char g_nation_cargo_capacity[4];  /* DGROUP:0x4b5c */
unsigned char g_nation_ship_count[4];      /* DGROUP:0x4b60 */
unsigned char g_nation_armed_ships[4];     /* DGROUP:0x4b64 */
unsigned char g_ai_military_target[4];     /* DGROUP:0x4b68 */
unsigned char g_nation_strength[4];        /* DGROUP:0x4b6c */
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
