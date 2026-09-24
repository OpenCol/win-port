/* create_MSWindowClass_window -- shipped at 1078:19f9.
 *
 * Ported by tools/port.py from matched/platform/create_MSWindowClass_window.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned far g_hinstance;                     DGROUP:0x56e2
 *   extern int g_window_count;                           DGROUP:0x3c7e
 *
 * The file's own MSWin dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: hbrush -> f8
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetSystemMetrics(col_i16 n);
extern col_u16 pascal CreateWindow(const char *cls, const char *title, col_u32 style,
                                        col_i16 x, col_i16 y, col_i16 w, col_i16 h, col_u16 parent, col_u16 menu,
                                        col_u16 hinst, void *param);
extern col_u16 pascal GetDC(col_u16 hwnd);
extern col_u16 pascal LoadCursor(col_u16 hinst, const char *name);
#endif

/* 0x26 bytes */


extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);




extern void push_word_5728(col_i16 v);

MSWin * create_MSWindowClass_window(char *title, col_u16 flag_word, col_i16 x, col_i16 y,
                                           col_i16 w, col_i16 h, MSWin *parent)
{
    register col_u16 flags = flag_word;
    register col_i16 ew;
    col_i16 hnd;
    MSWin *rec;
    col_i32 style;
    col_i16 caption;
    col_i16 eh;

    style = 0L;
    hnd = MSAllocateHandle(0x26L);
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
        rec->f1a = 1;
    else
        rec->f1a = 0;
    if (flags & 0x1000)
        rec->f1c = 1;
    else
        rec->f1c = 0;
    rec->hwnd = CreateWindow("MSWindowClass", title, style,
                             x == -1 ? 0x8000 : x,
                             y == -1 ? 0x8000 : y,
                             w == -1 ? 0x8000 : w + ew,
                             h == -1 ? 0x8000 : h + caption + eh - 1,
                             parent != 0 ? parent->hwnd : 0,
                             0, g_hinstance, 0L);
    push_word_5728(rec->hwnd);
    rec->hdc = GetDC(rec->hwnd);
    rec->hcursor = LoadCursor(0, (const char *)0x7f00L);
    rec->hpal = 0;
    rec->f22 = 0;
    rec->f8 = 0;
    rec->f6 = 0;
    rec->fe = 0L;
    rec->f12 = 0L;
    rec->f16 = 0L;
    rec->flags = flags;
    rec->f1e = 0;
    rec->f20 = 0;
    g_window_count++;
    return rec;
}
