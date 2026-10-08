/* PORT: after the write of GameBrains.step, w16_lockstep_step: the autoplay lockstep's checkpoint (W6); it does nothing outside a lockstep run. */
/* GameBrains_Init: candidate for 1008:2927 (141 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Marks the game in progress and hands the caller's record back in its empty state. When
 * the game came from a save -- `g_loaded_flag` at DGROUP:0x8b, which `load_game` sets --
 * it also rebuilds the per-nation goods-value table at SEG30:0x10 from each nation's own
 * market, four nations by sixteen goods, `set_europe` switching the market each
 * time round. `capture_cargo` is what reads that table back.
 *
 * The value written is the market sell price, `max_int(0, price[g] - 1)` -- the same
 * expression `europe_bid_price` is, spelled out rather than called.
 *
 * THE STORE IS `[BX+SI]`, not `[BX]`. The disassembler prints `MOV [es:bx+0x10],AL` and
 * emits the instruction as raw bytes because it cannot spell it: modrm 0x80 is
 * `[BX+SI+disp16]`. Read as printed, the inner loop would write sixteen values to one byte
 * and only the last would survive; read correctly it fills a sixteen-byte row per nation.
 *
 * Two globals are declared FAR in this module and in others: `g_loaded_flag`, which
 * `load_game.c` reaches DS-relative, and the `g_cur_nation_rec` pointer itself, which every other
 * reconstruction loads with a bare `LES BX,[0x4cc4]`. Both are `MOV AX,SEG / MOV ES,AX`
 * here. The declaration seam recorded in `struct-layouts.md`, twice in one function.
 *
 * The record the caller passes is `g_game_brains` at SEG20:0x40d6.
 *
 * RENAMED 2026-09-27 from begin_game: Mac GameMain calls begin_game and then GameBrains::Init(void). Windows run_game (1008:46ae) likewise calls two of these, ca99 and 2927. GameBrains::Init's whole Mac callee list is set_europe, and 2927's only callees are set_europe and a same-module max_int stub. Arity: the Mac method takes an implicit this, and the reconstruction takes GameBrains *p (g_game_brains, SEG20:0x40d6) and resets its fields (f0=-1, step=0, awaiting_human=0). This is an Init. Size is 160 Mac against 141 Windows. Layout: on Mac, Init is the first GameBrains method in CODE 11, directly before DoEndofTurn. On Windows, 2927 is directly before GameBrains_DoEndofTurn (1008:29b4), at the head of the GameBrains_* run (29b4..3cc8). The first binding was a descriptive guess with name_source descriptive and no Mac or witness evidence: the reconstructor read 'marks the game in progress' as the start of a game. The name has to move because ca99 takes begin_game.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_europe(int n);

/* same object: 4-byte `push cs; call ` */

extern void w16_lockstep_step(void *brains);

void GameBrains_Init(GameBrains *p)
{
    int nation;
    register int goods;

    g_game.game_in_progress = 1;
    if (g_loaded_flag != 0) {
        for (nation = 0; nation < 4; nation++) {
            set_europe(nation);
            for (goods = 0; goods < 0x10; goods++)
                g_goods_value[nation][goods] =
                    (unsigned char)max_int_1008_ce9c(0, g_cur_nation_rec->price[goods] - 1);
        }
    }
    p->awaiting_human = 0;
    p->step = 0;
    w16_lockstep_step(p);
    p->f0 = -1;
}
