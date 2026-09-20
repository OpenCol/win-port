/* winmain.c -- the entry point, and a placeholder for the real one.
 *
 * The shipped program enters at SEG1:0000, which is Borland's own Windows startup; that
 * startup calls WinMain. This repo links the same startup (C0WL.OBJ), so all it owes the
 * linker is a WinMain.
 *
 * WHAT THIS IS NOT. It is not the game's WinMain. That function registers the window
 * classes, builds the main window and runs the message loop, and it is not ported yet.
 * This exists so the link closes and so there is somewhere obvious for the real one to
 * land. It does the one thing that must happen before anything else reads a global:
 * applies the relocations the Windows loader used to apply to the data segments.
 */
#include <windows.h>
#include "col/segments.h"

int PASCAL WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show)
{
    (void)inst; (void)prev; (void)cmdline; (void)show;
    col_relocate();
    return 0;
}
