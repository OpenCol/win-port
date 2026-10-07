/* tutflag.h -- the tutorial hints already shown, as bits of SEG20:0x7788.
 *
 * Each hint is shown once. The code tests its bit, shows a TUTORIAL<n> message and sets the bit,
 * so these are the memory that makes "once" work -- and each names itself, because the resource
 * the guard shows is right beside the bit the guard sets.
 *
 * BIT INDEX IS THE TUTORIAL NUMBER PLUS THREE across all nine observed, but this header spells
 * them out rather than offering a macro, because TUTORIAL2 does NOT participate: `land_ho`
 * shows it unconditionally, relying on that function being once-only through a different latch
 * (bit 7 of the player's NationText once_flags). A formula would have promised a bit that is not there.
 *
 * THE WORD IS SHARED. Bits 1..3 of 0x7788 mirror the three sound settings (`game_menu_sound`), and
 * the tutorial latches start at bit 4. The tutorial's own ENABLE is elsewhere again --
 * GAMEFLAG_TUTORIAL, bit 7 of 0x7784 -- and TUTORIAL17's latch is GAMEFLAG_TUTORIAL17_SHOWN on
 * that same word. So the tutorial's state is spread over two words and this header covers one.
 */
#ifndef TUTFLAG_H
#define TUTFLAG_H

#define TUT_SHOWN_1         0x0010   /* "TUTORIAL1"  -- check_tut  */
#define TUT_SHOWN_3         0x0040   /* "TUTORIAL3"  -- check_tut  */
#define TUT_SHOWN_5         0x0100   /* "TUTORIAL5"  -- europe_new_turn  */
#define TUT_SHOWN_6         0x0200   /* "TUTORIAL6"  -- new_turn_colony  */
#define TUT_SHOWN_7         0x0400   /* "TUTORIAL7"  -- try_to_set_job   */
#define TUT_SHOWN_8         0x0800   /* "TUTORIAL8"  -- check_tut  */
#define TUT_SHOWN_9         0x1000   /* "TUTORIAL9"  -- check_tut  */
#define TUT_SHOWN_10        0x2000   /* "TUTORIAL10" -- check_tut  */
#define TUT_SHOWN_11        0x4000   /* "TUTORIAL11" -- check_tut  */

/* ---- the same word read as its HIGH BYTE ----------------------------------------------------
 * Two functions test SEG20:0x7789 -- byte 1 of this word -- because that is what the shipped
 * code does: `TEST BYTE [..7789],1` and not a word load and mask. The sites read it as
 * `HI_BYTE(g_game.hint_flags)`, and rewriting them as `g_game.hint_flags & TUT_SHOWN_*` would
 * move their bytes, so they get the same name shifted instead, as gameflag.h does.
 *
 * `europe_new_turn` is its own confirmation: it tests bit 0 of the high byte and then, two
 * lines later, sets TUT_SHOWN_5 -- 0x0100 -- on the word. One latch, read one way and written
 * the other.
 *
 * The other site, `move_unit`, tests 0x80 of the high byte, so 0x8000 of the word.
 * That bit has NO name here. The bit-index-plus-three rule would call it TUTORIAL12, but the
 * message is shown by 1010:e265, which is not reconstructed, so nothing in the corpus shows
 * the resource beside the bit -- which is the only evidence this header accepts.
 */
#define TUT_HI_SHOWN_5      (TUT_SHOWN_5 >> 8)   /* 0x01 */

#endif
