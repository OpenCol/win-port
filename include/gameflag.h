/* gameflag.h -- the bits of the game-state word at SEG20:0x7784.
 *
 * WHERE THE VALUES COME FROM. Nowhere but the code. Unlike terrain.h or building.h there is no
 * resource to re-derive these against, so this header meets the weaker bar tribes.h does and says
 * so: the NAMES are what the reconstructions established, bit by bit, and the VALUES are the masks
 * those functions test.
 *
 * ALMOST NOTHING HERE IS CHECKABLE, and the checker says so rather than pretending. The obvious
 * check -- "is this mask tested against that word?" -- is vacuous: every single bit from 0x1 to
 * 0x8000 is tested against it somewhere, so it passes whatever anyone writes here. It was written,
 * found useless and removed. What tools/check_headers.py still checks is that no two names claim
 * one bit and that each is a single bit. The MEANINGS rest on the reconstructions cited below.
 *
 * Each bit below is named only where a byte-verified reconstruction says what it does. The word is
 * masked with 0x4, 0x8, 0x400, 0x800, 0x4000 and 0x8000 elsewhere and NONE of those is named here,
 * because nothing establishes what they mean.
 *
 * ONE SHAPE TO WATCH. The word is reached three ways -- as `g_flags_7784` (a 16-bit word), as
 * `g_flag_7784` (its low byte) and as `g_flag_7785` (its HIGH byte). A mask against the high byte
 * is the word's mask >> 8, so the cheat menu is 0x20 there and 0x2000 here. Do not paste a word
 * constant onto a high-byte test.
 */
#ifndef GAMEFLAG_H
#define GAMEFLAG_H

/* Independence has been declared. The most-witnessed bit in the corpus: it suppresses the King's
 * cut, kills two reports, changes CREATE to CREATE2, stops Europe being reachable, and decides
 * whether `european_encounter` runs at all. */
#define GAMEFLAG_INDEPENDENCE       0x0001

/* A foreign power has intervened -- set by `intervention`. */
#define GAMEFLAG_INTERVENTION       0x0002

/* The independence advice has been given. `add_liberty` shows "AMBUSHHINT" and "CONSIDER" once a
 * nation is ready to hear it, then sets this so it is not offered again. */
#define GAMEFLAG_ADVICE_GIVEN       0x0004

/* The revolution has been WON. `end_of_year_checks` tests independence and the absence of this
 * bit, shows the King's defeat speech ("KINGLOSE") and sets it; `hall_of_fame_entry` reads it
 * beside the independence bit when it scores the game. */
#define GAMEFLAG_REVOLUTION_WON     0x0008

/* The war with the mother country has actually started. `resolve_attack` sets it the first time
 * an attack lands on the mother country after independence is declared, and `end_of_year_checks`
 * then multiplies the royal force totals by EIGHT rather than one. Declaring and fighting are
 * two different states and this is the second. */
#define GAMEFLAG_WAR_BEGUN          0x0040

/* The game has been scored once; `SCORED` asks whether to keep playing and this records that it
 * happened, whichever way the player answers. */
#define GAMEFLAG_SCORED             0x0010

/* TUTORIAL17 has been shown. A latch, not a setting: it is the tutorial hint's "once" flag. */
#define GAMEFLAG_TUTORIAL17_SHOWN   0x0020

/* The tutorial is on. `new_game_player_setup` sets it when the player picks difficulty 0, and the
 * hints test it before anything else. CONFIRMED independently by GAMEOPTIONS, whose last
 * checkbox is "~Tutorial Hints" and writes exactly this bit -- a name earned from call
 * sites and later met by a resource. */
#define GAMEFLAG_TUTORIAL           0x0080

/* ---- the top eight bits are the GAMEOPTIONS dialog ------------------------------------------
 * `get_game_options` (1048:b6ae) runs the GAMEOPTIONS resource over bits 0x8000..0x80 and rebuilds
 * the word from the boxes, so unlike the low bits these have a resource witness: the checkbox
 * labels name them, and tools/check_headers.py re-reads the resource and the dialog's masks.
 *
 * SLOT 6 IS INVERTED and only slot 6. It is set from `!(flags & 0x100)` and its bit is restored
 * when the box comes back CLEAR, so the stored bit is the NEGATION of the option shown -- one of
 * eight otherwise identical statements behaving differently. Its name carries NO_ for that reason.
 *
 * 0x2000 is deliberately absent: the dialog's masks are ~0xd000, ~0xf00 and ~0x80, which skip it,
 * and it is the cheat menu below -- a preference the player never sees.
 */
#define GAMEFLAG_SHOW_INDIAN_MOVES  0x8000  /* "Show ~Indian Moves"  */
#define GAMEFLAG_SHOW_FOREIGN_MOVES 0x4000  /* "Show ~Foreign Moves" */
#define GAMEFLAG_FAST_PIECE_SLIDE   0x1000  /* "Fast Piece ~Slide"   */
#define GAMEFLAG_END_OF_TURN        0x0800  /* "~End of Turn"        */
#define GAMEFLAG_AUTOSAVE           0x0400  /* "~Autosave"           */
#define GAMEFLAG_COMBAT_ANALYSIS    0x0200  /* "~Combat Analysis"    */
#define GAMEFLAG_NO_WATER_CYCLING   0x0100  /* "Water Color C~ycling", stored INVERTED */

/* The Cheat menu is on the menu bar. `cheat_menu_toggle` TOGGLES this -- it is the only bit here
 * that is flipped rather than set, and it is flipped in both arms of that function. */
#define GAMEFLAG_CHEAT_MENU         0x2000

#endif
