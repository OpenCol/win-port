/* PieceInit: candidate for 1040:2126 (52 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Builds the two small offscreen surfaces: a 12x13 SPRITE at SEG28:0x46, solid colour 0x31,
 * and a 32x32 GRPort at SEG28:0x66. They are not both ports -- `create_map_ports` defines
 * 0x46 as `Sprite g_flag_sprite` and `Sprite_dtor` tears it down, and it is where
 * `PieceDraw_noport` and `PieceDraw` put a nation flag to recolour it before drawing
 * it onto the port at 0x66.
 *
 * RENAMED 2026-09-27 from init_small_surfaces: CONST PROPOSAL. Mac PieceInit(void), CODE 22 0x1d6e, 64 bytes. Its seq is Sprite::Init, GRPort::Init, and its caller is letsGoSurfinNow. 2126 (void), 52 bytes, is Sprite_Init_wh(g_flag_sprite, 12, 13, 0x31), GRPort_Init_wh(g_icon_port, 32, 32); its caller is game_init (the letsGoSurfinNow slot, as for init_data). Layout: Mac CODE 22 runs PieceIconDraw(Sprite&) thunk, PieceInit, PieceDraw(GRPort&..); Windows 1040 runs map_draw_unit_marks (that thunk), 2126, PieceDraw (215a). Constants: overlap 1.0. tools/check_global_names.py:56 mentions init_small_surfaces in a comment only.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void Sprite_Init_wh(Sprite *p, int a, int b, unsigned char colour);

void PieceInit(void)
{
    Sprite_Init_wh(&g_flag_sprite, 12, 13, 0x31);
    GRPort_Init_wh(&g_icon_port, 32, 32);
}
