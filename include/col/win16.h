/* col/win16.h -- the Windows 3.1 types the model's declarations reach for, and the one
 * seam that lets the same source compile on a host for testing.
 *
 * Hand-written, not generated: it states what the PLATFORM provides, which is not
 * something model.json knows. Everything the game itself owns is in records.h.
 *
 * COL_HOST is defined by the host-native test harness (tests/), where there is no
 * windows.h and no 16-bit segmentation. Under Borland targeting Win16 it is not defined
 * and the real header is used, so the port cannot quietly drift from the platform.
 */
#ifndef COL_WIN16_H
#define COL_WIN16_H

#include "col/types.h" 

#ifndef COL_HOST
#  include <windows.h>
#else
#  define far
#  define near
#  define huge
#  define pascal
#  define cdecl
#  define _export
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef int            BOOL;
typedef unsigned char  BYTE;
typedef void far      *HWND;
typedef void far      *HDC;
typedef void far      *HANDLE;
typedef void far      *HINSTANCE;
typedef long (far pascal *WNDPROC)(HWND, WORD, WORD, long);
/* The Windows 3.1 structures whose layout the API fixes. w16types.h in include/ is the
 * byte-exact repo's copy of these and spells them with bare `int`, which is 16 bits
 * there and 32 here -- so a RECT laid over game memory would have the wrong offsets on
 * the host. These are the same structures at the same offsets, spelt so they keep them.
 * tools/port.py drops a file's `#include "w16types.h"`: windows.h covers it on Win16 and
 * this covers it here. */
typedef struct { col_i16 left, top, right, bottom; } RECT;
typedef struct { col_i16 x, y; } POINT;
typedef struct { col_i16 cx, cy; } SIZE;
typedef struct { unsigned char peRed, peGreen, peBlue, peFlags; } PALETTEENTRY;
typedef struct {
    col_u16 hdc;
    col_i16 fErase;
    RECT rcPaint;
    col_i16 fRestore, fIncUpdate;
    unsigned char rgbReserved[16];
} PAINTSTRUCT;
typedef struct {
    col_i16 lfHeight, lfWidth, lfEscapement, lfOrientation, lfWeight;
    unsigned char lfItalic, lfUnderline, lfStrikeOut, lfCharSet,
                  lfOutPrecision, lfClipPrecision, lfQuality, lfPitchAndFamily;
    char lfFaceName[32];
} LOGFONT;
#define W16TYPES_H          /* so a stray include of it is a no-op */
typedef struct { HWND hwnd; WORD message, wParam; DWORD lParam, time; POINT pt; } MSG;

typedef struct { BYTE rgbBlue, rgbGreen, rgbRed, rgbReserved; } RGBQUAD;
typedef struct { col_i16 tmHeight, tmAscent, tmDescent,
                     tmInternalLeading, tmExternalLeading;
                 unsigned char tmRest[22]; } TEXTMETRIC;
#endif

/* Records the byte-exact corpus names but never defines a body for: it reaches them only
 * through a pointer, so the port does the same. An incomplete type is the honest
 * spelling -- it keeps the name and makes a dereference a compile error rather than a
 * read of a layout nobody has established. */
typedef struct Dlg      Dlg;
typedef struct GameWin  GameWin;
typedef struct MrTimer  MrTimer;
typedef struct Rec4c28  Rec4c28;

#endif
