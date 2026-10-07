/* GameBrains_KeyboardEvent: candidate for 1008:37b8 (1263 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3
 * symbol: GameBrains_KeyboardEvent
 * name-source: sibling-build
 * size-source: 1263, not the census 1005. The last 32 bytes are the sixteen-entry jump table for
 * the letter keys 'A' to 'P'.
 *
 * EVERY UNIT ORDER ARRIVES HERE AS A KEY. The &Orders menu does not call the order routines; the
 * WM_COMMAND dispatcher turns each menu id into the equivalent CHARACTER and hands it to this
 * function, so the menu and the keyboard are the same code path and there is only one place that
 * decides whether an order is allowed.
 *
 * THREE KINDS OF KEY. Values 1 to 8 are taken before the switch and passed straight to
 * `GameBrains_MoveUnit` as a direction 0 to 7. Letters -- A, B, F, G, L, O, P, R, S, U, W
 * and the 0x144 that stands for Disband -- each check one byte of the order-availability table at
 * SEG30:0x50 and do nothing at all if it is clear, so a greyed menu item and an unusable key are
 * the same test. And two further ranges, 0xa1..0xa9 and 0xc0..0xc7, are the two directional key
 * sets; both map to the same eight directions but in DIFFERENT orders, and 0xa5 -- the centre of
 * the first set -- is simply absent.
 *
 * EVERY LETTER ORDER CLEARS the window's word at +4 before calling out, which is what takes the
 * unit out of "arrived" state; the two directional ranges do not.
 *
 * THE DUPLICATED PAIR is the thing to notice. Keys 0xd0 and 0xd1 appear TWICE, once in each arm of
 * the outer `if (end of turn pending && human turn active)` (SEG20:0x77c8, 0x77c6), with the same body both times -- and that body opens
 * with the same two-condition test again. In the first arm the test has already been made, so its
 * else-half, the one that opens a colony window or refreshes the unit, can never run there. The
 * two cases were written once and pasted into both arms, and only the copy in the second arm does
 * anything but clear the human-turn flag at SEG20:0x77c6.
 *
 * The call to `win_set_caption_1008_cf0f` on the first line returns a value that is thrown away.
 *
 * RENAMED 2026-09-26 from orders_key_command: the Mac build's GameBrains::KeyboardEvent(short): this plus a key; the menu handler routes the order keys to it. Every function of the GameBrains module (1008:2927..3cc8) is called on the one object g_game_brains (SEG20:0x40d6), and the width matches.
 */

#include "colonization.h"

#include "records.h"

typedef struct { char pad[1]; } StatWin;
extern StatWin g_stat_win;                /* SEG20:0x1566 */
typedef struct { char pad[1]; } ColonyWin;

extern GameState g_game;   /* SEG20:0x7782 */

extern void ColonyWin_Show(void *w, int c);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int win_set_caption_1008_cf0f(void *a, char *name);
extern int GameBrains_MoveUnit(GameBrains *o, int n);
extern int orders_hold(void);
extern int orders_activate(int x, int y);

extern int orders_sentry(void);
extern int orders_fortify(void);
extern int orders_road(void);
extern int orders_clear(void);
extern int orders_build(void);
extern int orders_load(void);
extern int orders_unload(void);
extern int orders_wait(void);
extern int orders_overboard(void);
extern int orders_disband(void);
extern int MSFlushInput_1008_cefc(void);

void GameBrains_KeyboardEvent(GameBrains *o, int key)
{
    register int colony;

    win_set_caption_1008_cf0f((char *)&g_stat_win, "Info Window");
    if (key > 0 && key < 9) {
        GameBrains_MoveUnit(o, key - 1);
        return;
    }
    if (g_game.end_of_turn_pending != 0 && g_game.human_turn_active != 0) {
        switch (key) {
        case 0xd1:
            if (g_game.end_of_turn_pending != 0 && g_game.human_turn_active != 0)
                g_game.human_turn_active = 0;
            else {
                o->phase = 0;
                orders_hold();
            }
            break;
        case 0xd0:
            if (g_game.end_of_turn_pending != 0 && g_game.human_turn_active != 0)
                g_game.human_turn_active = 0;
            else {
                colony = find_colony(g_cursor_x, g_cursor_y);
                if (colony >= 0)
                    if (g_colonies[colony].nation == ((unsigned char)g_game.view_nation) || g_game.reveal_all != 0)
                        ColonyWin_Show(&g_colony_window, colony);
            }
            break;
        }
    } else {
        switch (key) {
        case 0xd1:
            if (g_game.end_of_turn_pending != 0 && g_game.human_turn_active != 0)
                g_game.human_turn_active = 0;
            else {
                o->phase = 0;
                orders_hold();
            }
            break;
        case 0xd0:
            if (g_game.end_of_turn_pending != 0 && g_game.human_turn_active != 0)
                g_game.human_turn_active = 0;
            else {
                colony = find_colony(g_cursor_x, g_cursor_y);
                if (colony >= 0)
                    if (g_colonies[colony].nation == ((unsigned char)g_game.view_nation) || g_game.reveal_all != 0)
                        ColonyWin_Show(&g_colony_window, colony);
            }
            break;
        case 'A':
            if (g_order_flags[0] != 0) {
                o->phase = 0;
                orders_activate(g_cursor_x, g_cursor_y);
            }
            break;
        case 'G':
            if (g_order_flags[0xb] != 0) {
                o->phase = 0;
                orders_port();
            }
            break;
        case 'S':
            if (g_order_flags[4] != 0) {
                o->phase = 0;
                orders_sentry();
            }
            break;
        case 'F':
            if (g_order_flags[2] != 0) {
                o->phase = 0;
                orders_fortify();
            }
            break;
        case 'R':
            if (g_order_flags[7] != 0) {
                o->phase = 0;
                orders_road();
            }
            break;
        case 'P':
            if (g_order_flags[6] != 0) {
                o->phase = 0;
                orders_clear();
            }
            break;
        case 'B':
            if (g_order_flags[5] != 0) {
                o->phase = 0;
                orders_build();
            }
            break;
        case 'L':
            if (g_order_flags[8] != 0) {
                o->phase = 0;
                orders_load();
            }
            break;
        case 'U':
            if (g_order_flags[9] != 0) {
                o->phase = 0;
                orders_unload();
            }
            break;
        case 'W':
            if (g_order_flags[1] != 0) {
                o->phase = 0;
                orders_wait();
            }
            break;
        case 'O':
            if (g_order_flags[0xf] != 0) {
                o->phase = 0;
                orders_overboard();
            }
            break;
        case 0x144:
            if (g_order_flags[0x10] != 0) {
                o->phase = 0;
                orders_disband();
            }
            break;
        case 0xa8:
        case 0xc0:
            GameBrains_MoveUnit(o, 0);
            break;
        case 0xa4:
        case 0xc2:
            GameBrains_MoveUnit(o, 6);
            break;
        case 0xa6:
        case 0xc3:
            GameBrains_MoveUnit(o, 2);
            break;
        case 0xa2:
        case 0xc1:
            GameBrains_MoveUnit(o, 4);
            break;
        case 0xa7:
        case 0xc4:
            GameBrains_MoveUnit(o, 7);
            break;
        case 0xa9:
        case 0xc5:
            GameBrains_MoveUnit(o, 1);
            break;
        case 0xa1:
        case 0xc7:
            GameBrains_MoveUnit(o, 5);
            break;
        case 0xa3:
        case 0xc6:
            GameBrains_MoveUnit(o, 3);
            break;
        }
    }
    MSFlushInput_1008_cefc();
}
