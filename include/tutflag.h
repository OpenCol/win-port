/* tutflag.h -- the tutorial hints already shown, as bits of SEG20:0x7788.
 *
 * Each hint is shown once. The code tests its bit, shows a TUTORIAL<n> message and sets the bit,
 * so these are the memory that makes "once" work -- and each names itself, because the resource
 * the guard shows is right beside the bit the guard sets.
 *
 * BIT INDEX IS THE TUTORIAL NUMBER PLUS THREE across all nine observed, but this header spells
 * them out rather than offering a macro, because TUTORIAL2 does NOT participate: `tutorial_landfall`
 * shows it unconditionally, relying on that function being once-only through a different latch
 * (bit 7 of the player's NationRec52). A formula would have promised a bit that is not there.
 *
 * THE WORD IS SHARED. Bits 1..3 of 0x7788 mirror the three sound settings (`sound_options`), and
 * the tutorial latches start at bit 4. The tutorial's own ENABLE is elsewhere again --
 * GAMEFLAG_TUTORIAL, bit 7 of 0x7784 -- and TUTORIAL17's latch is GAMEFLAG_TUTORIAL17_SHOWN on
 * that same word. So the tutorial's state is spread over two words and this header covers one.
 */
#ifndef TUTFLAG_H
#define TUTFLAG_H

#define TUT_SHOWN_1         0x0010   /* "TUTORIAL1"  -- show_tutorial_hint  */
#define TUT_SHOWN_3         0x0040   /* "TUTORIAL3"  -- show_tutorial_hint  */
#define TUT_SHOWN_5         0x0100   /* "TUTORIAL5"  -- europe_nation_turn  */
#define TUT_SHOWN_6         0x0200   /* "TUTORIAL6"  -- colony_end_of_turn  */
#define TUT_SHOWN_7         0x0400   /* "TUTORIAL7"  -- colony_assign_job   */
#define TUT_SHOWN_8         0x0800   /* "TUTORIAL8"  -- show_tutorial_hint  */
#define TUT_SHOWN_9         0x1000   /* "TUTORIAL9"  -- show_tutorial_hint  */
#define TUT_SHOWN_10        0x2000   /* "TUTORIAL10" -- show_tutorial_hint  */
#define TUT_SHOWN_11        0x4000   /* "TUTORIAL11" -- show_tutorial_hint  */

#endif
