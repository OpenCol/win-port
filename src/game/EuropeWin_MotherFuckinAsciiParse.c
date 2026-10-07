/* EuropeWin_MotherFuckinAsciiParse: candidate for 1048:85c3 (124 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_MotherFuckinAsciiParse
 * name-source: sibling-build
 *
 * `[` and `]` move the tax rate down and up by one, in god mode only. The rate is the byte
 * at +1 of the nation record at DGROUP:0x4cc4 -- the same byte `king_adjust_tax` moves and
 * `traffic_to_europe` taxes the proceeds with -- and bit 5 of SEG20:0x7785 is god mode,
 * which `orders_disband` and `ReportWin_ReportsReligious` already call it.
 *
 * Nothing happens at all unless DGROUP:0x4cd2 is set, which gates the whole function before
 * the key is even looked at.
 *
 * The two arms are identical but for `DEC` and `INC`, and both are written out in full
 * rather than sharing a tail -- 39 bytes each, duplicated.
 *
 * IT IS A `switch`, not `if`/`else if`. The shipped runs BOTH comparisons before either
 * body -- `CMP AX,0x5b / JZ / CMP AX,0x5d / JZ / JMP` -- which is what a sparse switch emits;
 * an `if`/`else if` puts each body immediately after its own test and inverts the jump.
 *
 * The switch value is the widened key: `MOV AL,[bp+6] / MOV AH,0 / CMP AX,0x5b`. A `switch`
 * promotes its controlling expression, which is why the byte never gets compared in place.
 *
 * RENAMED 2026-09-27 from godmode_tax_keys: Mac EuropeWin::EuropeMotherFuckingAsciiParse(unsigned char), 164 bytes, against 124. SHORTENED: the Windows spelling EuropeWin_EuropeMotherFuckingAsciiParse is 39 characters; Borland keeps 32. The proposal drops the method's redundant leading 'Europe' (the class carries it) and the final g, which loses nothing that cannot be recovered. Alternatives: EuropeWin_EuropeMFAsciiParse (28) keeps the parallel with EuropeWin_EuropeKeyParse; the Borland truncation EuropeWin_EuropeMotherFuckingAsc is the least readable. Body: the Mac body is instruction for instruction the Windows one. Both gate on a modal word, switch on '[' 0x5b and ']' 0x5d, test god mode (0x2000 in the flag word), then tax-- or tax++ at +1 of the current nation, europe_change_mode, and DrawTitle on the global Europe window. The Mac reads the key at 8(a6), so it has no this either (a static handler). Arity: 1 on both. Callers: none on either build (installed by set_on_char in EuropeWin_Init). Layout: Mac europe_init, EuropeKeyParse, this, EuropeWin::Init; Windows europe_init, EuropeWin_EuropeKeyParse, 85c3, EuropeWin_Init.
 */

#include "colonization.h"

#include "gameflag.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void EuropeWin_DrawTitle(EuropeWin *buf);

/* same object: 4-byte `push cs; call ` */
extern "C" void europe_change_mode(void);

void EuropeWin_MotherFuckinAsciiParse(unsigned char key)
{
    if (g_modal_active == 0)
        return;
    switch (key) {
    case 0x5b:
        if (HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_CHEAT_MENU) {
            g_cur_nation_rec->tax--;
            europe_change_mode();
            EuropeWin_DrawTitle(&g_europe_win);
        }
        break;
    case 0x5d:
        if (HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_CHEAT_MENU) {
            g_cur_nation_rec->tax++;
            europe_change_mode();
            EuropeWin_DrawTitle(&g_europe_win);
        }
        break;
    }
}
