/* PORT: a record holding pointers is allocated by its sizeof (the game's size on i386, larger on 64-bit Windows) (W9). */
/* create_MSMovieClass_window: candidate for 1078:3010 (774 bytes).  Verdict: RELOCATABLE.
 * size-source: 775, not the census 766. The trailing 9 bytes are the dead second epilogue.
 *
 * Creates a game window of class "MSMovieClass" and returns its 0x26-byte record, allocated as a
 * locked memory handle whose first word is the handle itself. The caller's flag word becomes the
 * Windows style and the frame allowance:
 *
 *   4      a thick frame (the frame metrics twice over)
 *   2      a dialog frame, or with 0x40 a plain border
 *   1      a plain border (the border metrics twice over)
 *   8      minimize box   0x10 maximize box   0x20 system menu -- each also forces 0x40
 *   0x40   a caption; without it the caption height is not allowed for
 *   0x80   a vertical scroll bar   0x100 a horizontal one, each widening the allowance
 *   0x200  a child window, when there is a parent
 *   0x800  a pop-up, with the child bit taken back out
 *   0x400  and 0x1000 are recorded at +0x1a and +0x1c
 *
 * Every style starts from WS_CLIPCHILDREN. A position or size of -1 becomes CW_USEDEFAULT, and a
 * given width and height are grown by the frame, caption and scroll-bar allowances so they
 * describe the client area. Then the window's DC and the arrow cursor are recorded, the rest of
 * the record zeroed, the flag word kept at +0x24, and the window count at DGROUP:0x3c7e bumped.
 *
 * `create_MSWindowClass_window` at 1078:19f9 is the same function for "MSWindowClass"; here the
 * call to `push_ms_window` is the four-byte form, so that routine is in this object.
 *
 * THE FLAG WORD IS COPIED INTO A REGISTER LOCAL, not enregistered as a parameter. The shipped code
 * keeps it in SI and the frame allowance in DI. As a `register` parameter beside a `register` local
 * the local takes SI (rule 53); with the local left plain it is not promoted at all and gets a
 * stack slot the frame does not have. `register unsigned flags = flag_word;` declared first, with
 * `register int ew` second, is the spelling that gives SI then DI -- which also explains why the
 * load comes before the first statement.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void push_ms_window(int hwnd);

MSWin *create_MSMovieClass_window(char *title, unsigned flag_word, int x, int y,
                                           int w, int h, MSWin *parent)
{
    register unsigned flags = flag_word;
    register int ew;
    int hnd;
    MSWin *rec;
    long style;
    int caption;
    int eh;

    style = 0L;
    hnd = MSAllocateHandle((long)sizeof(MSWin));
    rec = (MSWin *)MSHLock(hnd);
    rec->handle = hnd;
    caption = GetSystemMetrics(4);
    style = 0x2000000L;
    if (flags & 4) {
        style |= 0x40000L;
        ew = GetSystemMetrics(0x20) * 2;
        eh = GetSystemMetrics(0x21) * 2;
    } else if ((flags & 2) && (flags & 0x40) == 0) {
        style |= 0x400000L;
        ew = GetSystemMetrics(0x20) * 2;
        eh = GetSystemMetrics(0x21) * 2;
    } else if ((flags & 2) && (flags & 0x40)) {
        style |= 0x800000L;
        ew = GetSystemMetrics(5) * 2;
        eh = GetSystemMetrics(6) * 2;
    } else if (flags & 1) {
        style |= 0x800000L;
        ew = GetSystemMetrics(5) * 2;
        eh = GetSystemMetrics(6) * 2;
    }
    if (flags & 8) {
        style |= 0x20000L;
        flags |= 0x40;
    }
    if (flags & 0x10) {
        style |= 0x10000L;
        flags |= 0x40;
    }
    if (flags & 0x20) {
        style |= 0x80000L;
        flags |= 0x40;
    }
    if (flags & 0x40)
        style |= 0xc00000L;
    else
        caption = 0;
    if (flags & 0x80) {
        style |= 0x200000L;
        ew += GetSystemMetrics(2) - 1;
    }
    if (flags & 0x100) {
        style |= 0x100000L;
        eh += GetSystemMetrics(3) - 1;
    }
    if ((flags & 0x200) && parent != 0)
        style |= 0x44000000L;
    if (flags & 0x800) {
        style |= 0x80000000L;
        style &= 0xbfffffffL;
    }
    if (flags & 0x400)
        rec->drag_by_client = 1;
    else
        rec->drag_by_client = 0;
    if (flags & 0x1000)
        rec->size_by_edges = 1;
    else
        rec->size_by_edges = 0;
    rec->hwnd = CreateWindow("MSMovieClass", title, style,
                             x == -1 ? 0x8000 : x,
                             y == -1 ? 0x8000 : y,
                             w == -1 ? 0x8000 : w + ew,
                             h == -1 ? 0x8000 : h + caption + eh - 1,
                             parent != 0 ? parent->hwnd : 0,
                             0, g_hinstance, 0L);
    push_ms_window(rec->hwnd);
    rec->hdc = GetDC(rec->hwnd);
    rec->hcursor = LoadCursor(0, (const char *)0x7f00L);
    rec->hpal = 0;
    rec->destroyed = 0;
    rec->hbrush = 0;
    rec->focus = 0;
    rec->pattern = 0L;
    rec->f12 = 0L;
    rec->icon = 0L;
    rec->flags = flags;
    rec->drag_height = 0;
    rec->edge_width = 0;
    g_window_count++;
    return rec;
}
