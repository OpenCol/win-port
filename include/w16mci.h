/* w16mci.h -- the MCI parameter blocks the game passes to mciSendCommand.
 *
 * GENERATED FROM BORLAND'S OWN MMSYSTEM.H, member for member, the way w16const.h is generated
 * from WINDOWS.H. `DWORD` is `unsigned long`, `UINT` is `unsigned` and `LPCSTR` is
 * `const char far *` under -ml, which is what the reconstructions already wrote by hand.
 *
 * WHY THESE AND NOT THE GAME'S OWN RECORDS. A struct may be shared only where every file that
 * USES a member agrees on its type, because a member's width and signedness change the generated
 * code. These are Windows API blocks with one published definition, so there is nothing to
 * disagree about -- unlike Unit or Colony, which stay per file (see include/README.md).
 *
 * ONE MEMBER NEEDED CHECKING. Six files declared MCI_STATUS_PARMS with `unsigned dwTrack;
 * unsigned pad;` where MMSYSTEM.H has `DWORD dwTrack` -- the same sixteen bytes, different types.
 * It only matters for a file that touches the member, and all three that do (`MSPlayCDTrack`,
 * `cd_next_track`, `cd_prev_track`) already declared it `unsigned long`. The
 * other six never name it, so they cannot tell.
 *
 * The device-specific blocks (MCI_ANIM_*, MCI_DGV_*) and the game's own MCI_OPEN_PARMS_EX are
 * NOT here: each is declared in exactly one file, so there is nobody to share with.
 *
 * C allows a typedef name once per translation unit, so a file that includes this must drop its
 * own copies. That is the whole cost of opting in.
 */
#ifndef W16MCI_H
#define W16MCI_H

typedef struct { unsigned long dwCallback; } MCI_GENERIC_PARMS;

typedef struct {
    unsigned long dwCallback;
    unsigned wDeviceID;
    unsigned wReserved0;
    const char far *lpstrDeviceType;
    const char far *lpstrElementName;
    const char far *lpstrAlias;
} MCI_OPEN_PARMS;

typedef struct {
    unsigned long dwCallback;
    unsigned long dwFrom;
    unsigned long dwTo;
} MCI_PLAY_PARMS;

typedef struct {
    unsigned long dwCallback;
    unsigned long dwTo;
} MCI_SEEK_PARMS;

typedef struct {
    unsigned long dwCallback;
    unsigned long dwTimeFormat;
    unsigned long dwAudio;
} MCI_SET_PARMS;

typedef struct {
    unsigned long dwCallback;
    unsigned long dwReturn;
    unsigned long dwItem;
    unsigned long dwTrack;
} MCI_STATUS_PARMS;

#endif
