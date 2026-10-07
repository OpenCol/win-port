/* LoadSaveGame: candidate for 1008:b24d (36 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 36, not the census 31. The trailing 5 bytes are the dead second epilogue.
 *
 * `load_game` reduced to a yes or no: 1 unless it returned 0. The result is built the long
 * way -- assume 1, clear it on failure -- rather than as `!= 0`, which is what puts the
 * `XOR SI,SI` behind a direct `JNZ` (rule 12's positive form).
 *
 * RENAMED 2026-09-27 from load_game_ok: Mac LoadSaveGame(void), 46 bytes, against 36. Caller: begin_game on both. Callee: load_game(void) only, on both. Layout: Mac save_game, LoadSaveGame, BuildSaveGameName; Windows save_game, return_zero_1008_b239 (a Windows-only stub), b24d, BuildSaveGameName. Body: load_game reduced to a yes/no. Arity: void.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */

int LoadSaveGame(void)
{
    register int ok = 1;

    if (load_game() == 0)
        ok = 0;
    return ok;
}
