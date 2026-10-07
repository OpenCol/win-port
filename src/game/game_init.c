/* game_init: candidate for 1008:4c16 (207 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: game_init
 * name-source: descriptive
 *
 * Start-up, in order: the full-screen backdrop is opened (`OpenBackdrop`), the cursor hidden for `show_opening` and
 * shown again, four module initialisers run, the small ports are built, twelve entries of the
 * four-byte table at SEG20:0x7746 are registered with the ids 0x80..0x8b, the random generator
 * is seeded THREE times, and the tick count is stamped into DGROUP:0x4bc2.
 *
 * THE THREE SEEDS ARE THE READABLE PART. Each is `return_zero_1040_74b9()` stored into one of
 * 0x4b86, 0x4b88 and 0x4b8a, then `randmize()`. The three saved values are all
 * zero in the shipped build, because the function that produces them returns nothing else --
 * so three seeds that once came from somewhere now come from the clock, and the three words
 * that recorded where are dead.
 *
 * A `GRPort` is constructed and destroyed around the whole thing and never used, which is what
 * the exception frame is for.
 *
 * `init_sprites`, `init_data` and `load_data` are reached by the five-byte NOP form
 * and are `extern`; `OpenBackdrop`, `empty_1008_4bec`, `cursor_load` and
 * `MSGetTicks_1008_cee5` by the four-byte form and are stubs here (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void init_sprites(void);
extern void init_data(void);
extern void load_data(void);
extern void PieceInit(void);

extern void randmize(void);

/* same object: 4-byte `push cs; call ` */
extern "C" void OpenBackdrop(void);

extern "C" void cursor_load(void *cursor, int id);

void game_init(void)
{
    GRPort port;
    register int i;

    OpenBackdrop();
    MSHideCursor();
    show_opening();
    MSShowCursor();
    empty_1008_4bec();
    init_sprites();
    init_data();
    load_data();
    PieceInit();
    for (i = 0; i < 12; i++)
        cursor_load(&g_cursors[i], i + 0x80);
    g_random_seed = return_zero_1040_74b9();
    randmize();
    g_event_seed = return_zero_1040_74b9();
    randmize();
    g_music_seed = return_zero_1040_74b9();
    randmize();
    g_seed_base = MSGetTicks_1008_cee5();
}

