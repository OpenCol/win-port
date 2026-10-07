/* EuropeWin_EuropeKeyParse: candidate for 1048:8557 (108 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeKeyParse
 * name-source: sibling-build
 *
 * `EuropeWin_MotherFuckinAsciiParse`'s neighbour 108 bytes above it, and the same shape: nothing happens at
 * all unless DGROUP:0x4cd2 is set, and the god-mode arm is gated on bit 5 of SEG20:0x7785 --
 * the same two gates, in the same order.
 *
 * Two keys. 0xd2 closes the Europe screen through `EuropeWin_OnOKClick(0)`, which ends its modal session. 0x134, in god mode, gives the current
 * nation a THOUSAND -- `ADD DWORD [es:bx+0xc31e],0x3e8` into the 0x13c-byte record the index
 * at DGROUP:0x4cc8 selects, whose array begins at SEG20:0xc2f4 (0xc31e is its +0x2a gold field) -- and then redraws through the same `europe_change_mode` and
 * `EuropeWin_DrawTitle(SEG20:0x2fd2)` pair its neighbour uses.
 *
 * The key is a WORD here where `EuropeWin_MotherFuckinAsciiParse` widens a byte: `MOV AX,[bp+6]`, so this
 * one's parameter is already an `int` and the values are above 0xff.
 *
 * IT IS A `switch`, not `if`/`else if`, for the reason its neighbour's header gives: the
 * shipped runs both comparisons before either body.
 *
 * RENAMED 2026-09-26 from godmode_money_keys: the Mac build's EuropeWin::EuropeKeyParse(short): EuropeWin_Init installs it as the window's keydown handler, and it takes the key -- the god-mode money keys are what it parses. The width matches.
 */

#include "colonization.h"

#include "gameflag.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
extern Nation g_nations[];           /* SEG20:0xc2f4, 0x13c bytes each;
                                            0xc31e is +0x2a, .gold */
extern GameState g_game;   /* SEG20:0x7782 */

extern void EuropeWin_DrawTitle(EuropeWin *buf);

/* same object: 4-byte `push cs; call ` */
extern "C" void europe_change_mode(void);
/* 5-byte NOP form, so extern (rule 47) */
extern void EuropeWin_OnOKClick(int n);

void EuropeWin_EuropeKeyParse(int key)
{
    if (g_modal_active == 0)
        return;
    switch (key) {
    case 0xd2:
        EuropeWin_OnOKClick(0);
        break;
    case 0x134:
        if (HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_CHEAT_MENU) {
            g_nations[g_cur_nation].gold += 1000;
            europe_change_mode();
            EuropeWin_DrawTitle(&g_europe_win);
        }
        break;
    }
}
