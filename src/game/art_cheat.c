/* PORT: this file's own declarations of GameWin_Init, Sprite_Init, set_on_close_1040_a7bc, win_center_1040_a778 removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 487 of the imported file. */
/* art_cheat: candidate for 1040:46aa (5574 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: art_cheat
 * name-source: sibling-build
 * size-source: 5574, not the census 359. The census stops at the first unconditional jump out of the
 * switch; the function runs to 1040:5c70 and its last 642 bytes are the switch's two tables.
 *
 * THE DEVELOPERS' PICTURE VIEWER, reached from the debug menu. It saves the game palette, asks
 * ARTISTS ("View which picture #?") for a number, opens a 640x480 window titled "Close Window to
 * Reset Palette", and draws whatever that number names. A number it does not know closes again at
 * once; anything else waits for the window to be closed and then puts the palette back.
 *
 * ONE HUNDRED AND SEVEN CASES, numbered 0..106 and fully unrolled. Eighty-one load a CVPC picture
 * straight into the window at a fixed size; the other twenty-six load picture 0x2ee as a backdrop and
 * then draw one SPRT sprite (0x2bc..0x2d4, 0x2c2 used twice) centred at 320,320. Each of those twenty-six has its OWN
 * rectangle local -- twenty-six of them, 8 bytes apart -- because `Sprite_Draw` returns its bounds
 * through an out parameter and nothing reuses one.
 *
 * WHAT THE BYTES SETTLED: the switch is on a LONG, DGROUP-at SEG31:0x2d4, and Borland compiles
 * that as a LINEAR SEARCH over a 428-byte table of 32-bit case values followed by a 214-byte table of
 * targets -- not the dense jump table the values 0..106 would otherwise get (rule 80). Copying the
 * long into a local first costs four more bytes of frame, because the compiler keeps its own
 * temporary regardless, so the `switch` has to read the global directly. The 107 arms were generated
 * from the disassembly rather than typed.
 *
 * RENAMED 2026-09-26 from debug_picture_viewer: the Mac build's art_cheat(void), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller MainMenuProc; callee CopyRawColors; callee Draw; callee Fill), the runner-up gets 3, and the arity agrees (0). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
/* PORT: this file's own Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, Sprite, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a-


class DlgControl;


#pragma option -a

#pragma option -a-

   /* 0x41e bytes: the palette, its handle and the cycling state */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;              /* SEG20:0x5ca */

struct logpal;
extern void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);
extern int popup_ask_number(char *ctx, char *name, int n);
extern int GRPort_LoadPicture(GRPort *port, int res, int a, int b, struct logpal *pal);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void show_window_1040_a79d(GameWin *w);

extern "C" void MSHideWindow_of_1040_a80c(void *w);

extern "C" void art_cheat(void)
{
    GameWin win;
    Sprite spr;
    RECT r0;
    RECT r1;
    RECT r2;
    RECT r3;
    RECT r4;
    RECT r5;
    RECT r6;
    RECT r7;
    RECT r8;
    RECT r9;
    RECT r10;
    RECT r11;
    RECT r12;
    RECT r13;
    RECT r14;
    RECT r15;
    RECT r16;
    RECT r17;
    RECT r18;
    RECT r19;
    RECT r20;
    RECT r21;
    RECT r22;
    RECT r23;
    RECT r24;
    RECT r25;
    char pal[768];
    register int unknown;

    unknown = 0;
    CTable_CopyRawColors((CTable *)((struct logpal *)&g_game_palette), pal, 0xa, 0xec);
    if (popup_ask_number("DEBUG", "ARTISTS", 1) == 0) {
        GameWin_Init(&win, "Close Window to Reset Palette", 0x69, 0, 0,
                                         0x280, 0x1e0, &g_game_palette, (Screen *)&g_main_win);
        switch (g_text_args[0]) {
    case 0:
        GRPort_LoadPicture(&win, 0xc9, 0x28, 0x60, (struct logpal *)&g_game_palette);
        break;
    case 1:
        GRPort_LoadPicture(&win, 0xca, 0x10, 0x18, (struct logpal *)&g_game_palette);
        break;
    case 2:
        GRPort_LoadPicture(&win, 0xcb, 0x10, 0x18, (struct logpal *)&g_game_palette);
        break;
    case 3:
        GRPort_LoadPicture(&win, 0xcc, 0x28, 0x66, (struct logpal *)&g_game_palette);
        break;
    case 4:
        GRPort_LoadPicture(&win, 0xfa, 0x28, 0x60, (struct logpal *)&g_game_palette);
        break;
    case 5:
        GRPort_LoadPicture(&win, 0xfb, 0x28, 0x60, (struct logpal *)&g_game_palette);
        break;
    case 6:
        GRPort_LoadPicture(&win, 0xfc, 0xef, 0x7, (struct logpal *)&g_game_palette);
        break;
    case 7:
        GRPort_LoadPicture(&win, 0x12c, 0x28, 0x40, (struct logpal *)&g_game_palette);
        break;
    case 8:
        GRPort_LoadPicture(&win, 0x12d, 0x28, 0x40, (struct logpal *)&g_game_palette);
        break;
    case 9:
        GRPort_LoadPicture(&win, 0x12e, 0x28, 0x40, (struct logpal *)&g_game_palette);
        break;
    case 10:
        GRPort_LoadPicture(&win, 0x12f, 0x28, 0x40, (struct logpal *)&g_game_palette);
        break;
    case 11:
        GRPort_LoadPicture(&win, 0x130, 0x28, 0x40, (struct logpal *)&g_game_palette);
        break;
    case 12:
        GRPort_LoadPicture(&win, 0x131, 0x28, 0x40, (struct logpal *)&g_game_palette);
        break;
    case 13:
        GRPort_LoadPicture(&win, 0x132, 0x28, 0x40, (struct logpal *)&g_game_palette);
        break;
    case 14:
        GRPort_LoadPicture(&win, 0x133, 0x8e, 0x40, (struct logpal *)&g_game_palette);
        break;
    case 15:
        GRPort_LoadPicture(&win, 0x136, 0xa, 0xdc, (struct logpal *)&g_game_palette);
        break;
    case 16:
        GRPort_LoadPicture(&win, 0x190, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 17:
        GRPort_LoadPicture(&win, 0x191, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 18:
        GRPort_LoadPicture(&win, 0x192, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 19:
        GRPort_LoadPicture(&win, 0x193, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 20:
        GRPort_LoadPicture(&win, 0x194, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 21:
        GRPort_LoadPicture(&win, 0x195, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 22:
        GRPort_LoadPicture(&win, 0x196, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 23:
        GRPort_LoadPicture(&win, 0x197, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 24:
        GRPort_LoadPicture(&win, 0x198, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 25:
        GRPort_LoadPicture(&win, 0x199, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 26:
        GRPort_LoadPicture(&win, 0x19a, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 27:
        GRPort_LoadPicture(&win, 0x19b, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 28:
        GRPort_LoadPicture(&win, 0x19c, 0x31, 0x8a, (struct logpal *)&g_game_palette);
        break;
    case 29:
        GRPort_LoadPicture(&win, 0x1c2, 0xa, 0x27, (struct logpal *)&g_game_palette);
        break;
    case 30:
        GRPort_LoadPicture(&win, 0x1c3, 0x10, 0x24, (struct logpal *)&g_game_palette);
        break;
    case 31:
        GRPort_LoadPicture(&win, 0x1f4, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 32:
        GRPort_LoadPicture(&win, 0x1f5, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 33:
        GRPort_LoadPicture(&win, 0x1f6, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 34:
        GRPort_LoadPicture(&win, 0x1f7, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 35:
        GRPort_LoadPicture(&win, 0x1f8, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 36:
        GRPort_LoadPicture(&win, 0x1f9, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 37:
        GRPort_LoadPicture(&win, 0x1fa, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 38:
        GRPort_LoadPicture(&win, 0x1fb, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 39:
        GRPort_LoadPicture(&win, 0x1fc, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 40:
        GRPort_LoadPicture(&win, 0x1fd, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 41:
        GRPort_LoadPicture(&win, 0x1fe, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 42:
        GRPort_LoadPicture(&win, 0x1ff, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 43:
        GRPort_LoadPicture(&win, 0x200, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 44:
        GRPort_LoadPicture(&win, 0x201, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 45:
        GRPort_LoadPicture(&win, 0x202, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 46:
        GRPort_LoadPicture(&win, 0x203, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 47:
        GRPort_LoadPicture(&win, 0x204, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 48:
        GRPort_LoadPicture(&win, 0x205, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 49:
        GRPort_LoadPicture(&win, 0x206, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 50:
        GRPort_LoadPicture(&win, 0x207, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 51:
        GRPort_LoadPicture(&win, 0x208, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 52:
        GRPort_LoadPicture(&win, 0x209, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 53:
        GRPort_LoadPicture(&win, 0x20a, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 54:
        GRPort_LoadPicture(&win, 0x20b, 0x34, 0xc2, (struct logpal *)&g_game_palette);
        break;
    case 55:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        break;
    case 56:
        GRPort_LoadPicture(&win, 0x44c, 0x28, 0x9e, (struct logpal *)&g_game_palette);
        break;
    case 57:
        GRPort_LoadPicture(&win, 0x44d, 0x28, 0xa8, (struct logpal *)&g_game_palette);
        break;
    case 58:
        GRPort_LoadPicture(&win, 0x44e, 0x28, 0xc0, (struct logpal *)&g_game_palette);
        break;
    case 59:
        GRPort_LoadPicture(&win, 0x4b0, 0xa, 0xec, (struct logpal *)&g_game_palette);
        break;
    case 60:
        GRPort_LoadPicture(&win, 0x4b1, 0xa, 0xec, (struct logpal *)&g_game_palette);
        break;
    case 61:
        GRPort_LoadPicture(&win, 0x4b2, 0xa, 0xec, (struct logpal *)&g_game_palette);
        break;
    case 62:
        GRPort_LoadPicture(&win, 0x4b3, 0xa, 0xec, (struct logpal *)&g_game_palette);
        break;
    case 63:
        GRPort_LoadPicture(&win, 0x4b4, 0xa, 0xec, (struct logpal *)&g_game_palette);
        break;
    case 64:
        GRPort_LoadPicture(&win, 0x4b5, 0xa, 0xec, (struct logpal *)&g_game_palette);
        break;
    case 65:
        GRPort_LoadPicture(&win, 0x4b6, 0xa, 0xec, (struct logpal *)&g_game_palette);
        break;
    case 66:
        GRPort_LoadPicture(&win, 0x4b7, 0xa, 0xec, (struct logpal *)&g_game_palette);
        break;
    case 67:
        GRPort_LoadPicture(&win, 0x4b8, 0xa, 0xec, (struct logpal *)&g_game_palette);
        break;
    case 68:
        GRPort_LoadPicture(&win, 0x4b9, 0xa, 0xec, (struct logpal *)&g_game_palette);
        break;
    case 69:
        GRPort_LoadPicture(&win, 0x514, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        break;
    case 70:
        GRPort_LoadPicture(&win, 0x515, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        break;
    case 71:
        GRPort_LoadPicture(&win, 0x516, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        break;
    case 72:
        GRPort_LoadPicture(&win, 0x517, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        break;
    case 73:
        GRPort_LoadPicture(&win, 0x518, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        break;
    case 74:
        GRPort_LoadPicture(&win, 0x51a, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        break;
    case 75:
        GRPort_LoadPicture(&win, 0x51b, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        break;
    case 76:
        GRPort_LoadPicture(&win, 0x51c, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        break;
    case 77:
        GRPort_LoadPicture(&win, 0x51d, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        break;
    case 78:
        GRPort_LoadPicture(&win, 0x51e, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        break;
    case 79:
        GRPort_LoadPicture(&win, 0x51f, 0xa, 0xe3, (struct logpal *)&g_game_palette);
        break;
    case 80:
        GRPort_LoadPicture(&win, 0x311, 0x28, 0x60, (struct logpal *)&g_game_palette);
        break;
    case 81:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2bc);
        Sprite_Draw(&r0, &spr, &win, 0x140, 0x140);
        break;
    case 82:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2bd);
        Sprite_Draw(&r1, &spr, &win, 0x140, 0x140);
        break;
    case 83:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2be);
        Sprite_Draw(&r2, &spr, &win, 0x140, 0x140);
        break;
    case 84:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2bf);
        Sprite_Draw(&r3, &spr, &win, 0x140, 0x140);
        break;
    case 85:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2c0);
        Sprite_Draw(&r4, &spr, &win, 0x140, 0x140);
        break;
    case 86:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2c1);
        Sprite_Draw(&r5, &spr, &win, 0x140, 0x140);
        break;
    case 87:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2c2);
        Sprite_Draw(&r6, &spr, &win, 0x140, 0x140);
        break;
    case 88:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2c2);
        Sprite_Draw(&r7, &spr, &win, 0x140, 0x140);
        break;
    case 89:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2c3);
        Sprite_Draw(&r8, &spr, &win, 0x140, 0x140);
        break;
    case 90:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2c4);
        Sprite_Draw(&r9, &spr, &win, 0x140, 0x140);
        break;
    case 91:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2c5);
        Sprite_Draw(&r10, &spr, &win, 0x140, 0x140);
        break;
    case 92:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2c6);
        Sprite_Draw(&r11, &spr, &win, 0x140, 0x140);
        break;
    case 93:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2c7);
        Sprite_Draw(&r12, &spr, &win, 0x140, 0x140);
        break;
    case 94:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2c8);
        Sprite_Draw(&r13, &spr, &win, 0x140, 0x140);
        break;
    case 95:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2c9);
        Sprite_Draw(&r14, &spr, &win, 0x140, 0x140);
        break;
    case 96:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2ca);
        Sprite_Draw(&r15, &spr, &win, 0x140, 0x140);
        break;
    case 97:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2cb);
        Sprite_Draw(&r16, &spr, &win, 0x140, 0x140);
        break;
    case 98:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2cc);
        Sprite_Draw(&r17, &spr, &win, 0x140, 0x140);
        break;
    case 99:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2cd);
        Sprite_Draw(&r18, &spr, &win, 0x140, 0x140);
        break;
    case 100:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2ce);
        Sprite_Draw(&r19, &spr, &win, 0x140, 0x140);
        break;
    case 101:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2cf);
        Sprite_Draw(&r20, &spr, &win, 0x140, 0x140);
        break;
    case 102:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2d0);
        Sprite_Draw(&r21, &spr, &win, 0x140, 0x140);
        break;
    case 103:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2d1);
        Sprite_Draw(&r22, &spr, &win, 0x140, 0x140);
        break;
    case 104:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2d2);
        Sprite_Draw(&r23, &spr, &win, 0x140, 0x140);
        break;
    case 105:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2d3);
        Sprite_Draw(&r24, &spr, &win, 0x140, 0x140);
        break;
    case 106:
        GRPort_LoadPicture(&win, 0x2ee, 0x10, 0xd2, (struct logpal *)&g_game_palette);
        Sprite_Init(&spr, 0x2d4);
        Sprite_Draw(&r25, &spr, &win, 0x140, 0x140);
        break;
        default:
            unknown = 1;
            break;
        }
        if (unknown == 0) {
            win_center_1040_a778((DialogWin *)((char *)&win + 0x26));
            show_window_1040_a79d(&win);
            set_on_close_1040_a7bc((EventTable *)((char *)&win + 0x33), (void*)(viewer_dismiss_handler));
            g_viewer_dismissed = 0;
            while (g_viewer_dismissed == 0)
                MSDoTask_1040_a7f5();
            MSHideWindow_of_1040_a80c((DialogWin *)((char *)&win + 0x26));
        }
    }
    CTable_Fill(&g_game_palette, 0xa, 0xec, pal);
}
