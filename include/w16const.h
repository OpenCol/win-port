/* w16const.h -- Windows 3.1 API constants, as the Borland C++ 4.52 WINDOWS.H spells them.
 *
 * GENERATED from analysis/toolchain/BC452/INCLUDE/WINDOWS.H, token for token, so a file may
 * include this and WINDOWS.H together without a redefinition. tools/check_headers.py fails if
 * any value here stops agreeing with that header.
 *
 * Constants only. The API PROTOTYPES stay in each reconstruction: whether a handle is `int` or
 * `unsigned` changes the code a comparison compiles to (JL against JB), and the corpus has
 * matched both, so one shared prototype would silently break half of it.
 */
#ifndef W16CONST_H
#define W16CONST_H


/* ---- window messages ----------------------------------------------------------------------- */
#define WM_CREATE            0x0001
#define WM_DESTROY           0x0002
#define WM_MOVE              0x0003
#define WM_SIZE              0x0005
#define WM_ACTIVATE          0x0006
#define WM_SETFOCUS          0x0007
#define WM_KILLFOCUS         0x0008
#define WM_PAINT             0x000F
#define WM_CLOSE             0x0010
#define WM_ERASEBKGND        0x0014
#define WM_GETMINMAXINFO     0x0024
#define WM_KEYDOWN           0x0100
#define WM_KEYUP             0x0101
#define WM_CHAR              0x0102
#define WM_INITDIALOG        0x0110
#define WM_COMMAND           0x0111
#define WM_SYSCOMMAND        0x0112
#define WM_TIMER             0x0113
#define WM_HSCROLL           0x0114
#define WM_VSCROLL           0x0115
#define WM_MOUSEMOVE         0x0200
#define WM_LBUTTONDOWN       0x0201
#define WM_LBUTTONUP         0x0202
#define WM_LBUTTONDBLCLK     0x0203
#define WM_RBUTTONDOWN       0x0204
#define WM_RBUTTONUP         0x0205
#define WM_QUERYNEWPALETTE   0x030F
#define WM_PALETTECHANGED    0x0311
#define WM_USER              0x0400

/* ---- scroll bars --------------------------------------------------------------------------- */
#define SB_HORZ              0
#define SB_VERT              1
#define SB_CTL               2
#define SB_LINEUP            0
#define SB_LINEDOWN          1
#define SB_PAGEUP            2
#define SB_PAGEDOWN          3
#define SB_THUMBPOSITION     4
#define SB_THUMBTRACK        5
#define SB_TOP               6
#define SB_BOTTOM            7
#define SB_ENDSCROLL         8

/* ---- GetWindow / GetWindowLong / GetClassWord indices -------------------------------------- */
#define GW_HWNDFIRST         0
#define GW_HWNDLAST          1
#define GW_HWNDNEXT          2
#define GW_HWNDPREV          3
#define GW_OWNER             4
#define GW_CHILD             5
#define GWL_WNDPROC          (-4)
#define GWW_HINSTANCE        (-6)
#define GWW_HWNDPARENT       (-8)
#define GWW_ID               (-12)
#define GWL_STYLE            (-16)
#define GWL_EXSTYLE          (-20)
#define GCW_HBRBACKGROUND    (-10)
#define GCW_CBWNDEXTRA       (-18)
#define GCW_CBCLSEXTRA       (-20)
#define GCL_WNDPROC          (-24)

/* ---- window and class styles --------------------------------------------------------------- */
#define WS_POPUP             0x80000000L
#define WS_CHILD             0x40000000L
#define WS_VISIBLE           0x10000000L
#define WS_BORDER            0x00800000L
#define WS_TABSTOP           0x00010000L
#define CS_VREDRAW           0x0001
#define CS_HREDRAW           0x0002
#define CS_DBLCLKS           0x0008
#define CS_OWNDC             0x0020
#define SW_HIDE              0
#define SW_SHOWNORMAL        1
#define SW_SHOW              5

/* ---- GDI ----------------------------------------------------------------------------------- */
#define HORZRES              8
#define VERTRES              10
#define BITSPIXEL            12
#define NUMCOLORS            24
#define RASTERCAPS           38
#define SIZEPALETTE          104
#define NUMRESERVED          106
#define RC_PALETTE           0x0100
#define PC_RESERVED          0x01
#define PC_EXPLICIT          0x02
#define PC_NOCOLLAPSE        0x04
#define PS_SOLID             0
#define TRANSPARENT          1
#define OPAQUE               2
#define WHITE_BRUSH          0
#define LTGRAY_BRUSH         1
#define GRAY_BRUSH           2
#define DKGRAY_BRUSH         3
#define BLACK_BRUSH          4
#define NULL_BRUSH           5
#define SRCCOPY              0x00CC0020L
#define DT_LEFT              0x0000
#define DT_CENTER            0x0001
#define DT_VCENTER           0x0004
#define DT_WORDBREAK         0x0010
#define DT_SINGLELINE        0x0020
#define DT_CALCRECT          0x0400

/* ---- keys, message boxes, memory ----------------------------------------------------------- */
#define VK_SHIFT             0x10
#define VK_TAB               0x09
#define VK_SPACE             0x20
#define VK_RETURN            0x0D
#define VK_ESCAPE            0x1B
#define VK_LEFT              0x25
#define VK_UP                0x26
#define VK_RIGHT             0x27
#define VK_DOWN              0x28
#define VK_NUMPAD2           0x62
#define VK_NUMPAD4           0x64
#define VK_NUMPAD6           0x66
#define VK_NUMPAD8           0x68
#define MB_OK                0x0000
#define GMEM_MOVEABLE        0x0002

/* Messages the reconstructions switch on that were not listed here before. Same source as
 * the rest of this file -- Borland's own headers, token for token -- with MM_MCINOTIFY
 * from MMSYSTEM.H, which is where the MCI notification the CD code waits on lives.
 * Looked up by VALUE and filtered by prefix: WINDOWS.H has several unrelated macros per
 * number, and a value-only search returned TEXTCAPS for 0x22 and ERR_CREATEWND for 0x44. */
#define WM_CHILDACTIVATE     0x0022
#define WM_COMMNOTIFY        0x0044
#define WM_NCHITTEST         0x0084
#define WM_NCLBUTTONDOWN     0x00A1
#define WM_NCRBUTTONDOWN     0x00A4
#define WM_PARENTNOTIFY      0x0210
#define WM_CUT               0x0300
#define SC_MINIMIZE          0xF020
#define SC_RESTORE           0xF120
#define MM_MCINOTIFY         0x3B9

#endif
