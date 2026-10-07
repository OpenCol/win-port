/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 81 of the imported file. */
/* GRPort_ChangeColor: candidate for 1068:51a8 (296 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_ChangeColor
 *
 * Replaces colour `from` with `to` in every pixel of a port, clipped to a rectangle. The
 * function names itself: when the port is not already locked it prints
 * "Warning: Port not locked in ChangeColor" (DGROUP:0x2eb1) through the same reporter and
 * the same context byte `Sprite_ChangeColor` uses, locks the port itself, and unlocks it
 * again at the end -- but only if it was the one that locked it.
 *
 * The area is `IntersectRect(&r, clip, &port->rect)`, the port's live rectangle being the
 * one at +8 that `GRPort_ResetClip` copies into. Then `GRPort_PixelMap` gives the
 * first pixel and the two nested loops walk `rect_width_1068_96a9(&r)` columns by
 * `rect_height_1068_96c8(&r)` rows -- the rectangle's width and height, and
 * `matched/Sprite_ChangeColor.c` already uses the second one for a row count. Both are
 * re-called on every iteration as the loop bound, which is what `-Od` does with a call in
 * a condition and is not something the source has to spell specially.
 *
 * THE PIXEL POINTER IS HUGE, and the shipped bytes say so twice. `q++` compiles to
 * `ADD WORD [q],1 / ADC AX,0 / MOV CL,__AHSHIFT / SHL AX,CL / ADD [q+2],AX` -- carry out of
 * the offset turned into selector steps -- and the row reset compiles to the same shape
 * around a subtraction. A `` pointer would simply wrap at 64K. This is the same rule 38
 * that `MSLockGraf` turned on, from the other direction: there the -ness showed up in a
 * null test, here in the arithmetic, and a port surface bigger than a segment is why both.
 *
 * `from` and `to` are `unsigned char`: the shipped compare is `CMP AL,[BP+0xe]`, eight bits
 * wide, and the store is a byte. Declared `int` (or `char`, against an `unsigned char`
 * pixel) both sides promote and the compare becomes 16-bit -- rule 27, and the parameter
 * slots are still two bytes wide either way, so the call sites cannot tell you.
 *
 * The order inside the not-locked branch is message, then flag, then lock. That is the
 * opposite order from `Sprite_ChangeColor`, which sets its flag first; the two are visibly
 * the same routine written twice for different records, and they do not agree line for
 * line.
 *
 * Six of the callees arrive as the compiler's own 4-byte PUSH CS / CALL NEAR and so are
 * defined here as stubs; `GRPort_PixelMap` and `GRPort_NextScanLine` are TLINK's 5-byte form and
 * stay `extern`. `IntersectRect` is a Win16 API and must be declared ``: its
 * arguments are pushed LEFT to right and the callee clears the stack, so there is no
 * `ADD SP` after the call, and declaring it `` reverses all three pushes.
 *
 * NAME: the function's own message. `1068:517c`, its only caller, is 44 bytes and is the
 * overload that supplies the rectangle -- the Macintosh build carries exactly that pair for
 * the neighbouring operation, `GRPort::GRPort_FillColor(Rect*, short)` and `GRPort::GRPort_FillColor(short)`.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern char *GRPort_PixelMap(GRPort *port, int x, int y);
extern unsigned char *GRPort_NextScanLine(GRPort *p, unsigned char *row);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */
extern "C" char GRPort_IsLocked(GRPort *p);
extern "C" void report_error_1068_963d(void *ctx, char *msg);
extern "C" void MSLockGraf(GRPort *port);
extern "C" void MSUnlockGraf(GRPort *port);
    /* the rectangle's width */
    /* the rectangle's height */

void GRPort_ChangeColor(GRPort *p, RECT *clip,
                                      unsigned char from, unsigned char to)
{
    int w;
    unsigned char *q;
    RECT r;
    int locked;
    register int x;
    register int y;

    locked = 1;
    if (!GRPort_IsLocked(p)) {
        report_error_1068_963d(&g_error_log, "Warning: Port not locked in ChangeColor");
        locked = 0;
        MSLockGraf(p);
    }
    IntersectRect(&r, clip, &p->rect);
    w = rect_width_1068_96a9(&r);
    q = (unsigned char*)(GRPort_PixelMap(p, r.left, r.top));
    for (y = 0; rect_height_1068_96c8(&r) > y; y++) {
        for (x = 0; rect_width_1068_96a9(&r) > x; x++) {
            if (*q == from)
                *q = to;
            q++;
        }
        q = GRPort_NextScanLine(p, q) - w;
    }
    if (locked == 0)
        MSUnlockGraf(p);
}
