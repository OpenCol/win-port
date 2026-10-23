/* KERNEL's file functions (roadmap W4). Win32 keeps OpenFile, _lclose, _llseek, _hread and
   _hwrite, with Win16's OFSTRUCT layout and OF_* flags; the file handle the game keeps in a
   `short` is a 16-bit token. */
#include "w16.h"

#include <stdio.h>
#include <string.h>

/* OpenFile for a file whose full path is OFS_MAXPATHNAME (128) characters or more, which Win32's
   OpenFile refuses as Win16's did. The kit keeps a Steam or GOG game it unpacked in a folder of
   its own (src/launcher), which can be deep: inside Steam's own folder a save's full path is
   about 127 characters, and longer under Wine. The game asks for five things (OF_READ,
   OF_READWRITE, OF_CREATE | OF_READWRITE, OF_EXIST, and OF_DELETE/OF_PARSE nowhere but kept
   here); the OFSTRUCT gets the path as far as it fits, which the game never reads back. */
static HFILE open_long(const char *full, OFSTRUCT *o, unsigned mode)
{
    DWORD access, share, a;
    HANDLE h;
    if (o) {
        memset(o, 0, sizeof *o);
        o->cBytes = sizeof *o;
        o->fFixedDisk = 1;
        snprintf(o->szPathName, sizeof o->szPathName, "%s", full);
    }
    if (mode & OF_PARSE)
        return 0;
    if (mode & OF_DELETE)
        return DeleteFileA(full) ? 1 : HFILE_ERROR;
    if (mode & OF_EXIST) {
        a = GetFileAttributesA(full);
        if (a == INVALID_FILE_ATTRIBUTES || (a & FILE_ATTRIBUTE_DIRECTORY)) {
            if (o)
                o->nErrCode = ERROR_FILE_NOT_FOUND;
            return HFILE_ERROR;
        }
        return 1;
    }
    access = (mode & 3) == OF_WRITE ? GENERIC_WRITE : (mode & 3) == OF_READWRITE ? GENERIC_READ | GENERIC_WRITE
                                                                                 : GENERIC_READ;
    switch (mode & 0x70) {
    case OF_SHARE_EXCLUSIVE: share = 0; break;
    case OF_SHARE_DENY_WRITE: share = FILE_SHARE_READ; break;
    case OF_SHARE_DENY_READ: share = FILE_SHARE_WRITE; break;
    default: share = FILE_SHARE_READ | FILE_SHARE_WRITE; break;  /* compatibility, deny none */
    }
    h = CreateFileA(full, access, share, 0, (mode & OF_CREATE) ? CREATE_ALWAYS : OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL, 0);
    if (h == INVALID_HANDLE_VALUE) {
        if (o)
            o->nErrCode = (WORD)GetLastError();
        return HFILE_ERROR;
    }
    return (HFILE)(intptr_t)h;            /* what Win32's OpenFile returns: _lclose and _hread take it */
}

short w16_OpenFile(const char *name, void *ofs, unsigned mode) W16(OpenFile);
short w16_OpenFile(const char *name, void *ofs, unsigned mode)
{
    char full[2 * MAX_PATH];
    DWORD n = GetFullPathNameA(name, sizeof full, full, 0);
    HFILE f = n >= OFS_MAXPATHNAME && n < sizeof full ? open_long(full, (OFSTRUCT *)ofs, mode)
                                                      : OpenFile(name, (OFSTRUCT *)ofs, mode);
    w16_trace("OpenFile(%s, %#x) = %d", name, mode, f);
    if (f == HFILE_ERROR)
        return -1;
    if (mode & (OF_EXIST | OF_PARSE | OF_DELETE))
        return 1;                       /* no handle stays open; the game tests the sign */
    return (short)w16_token((HANDLE)(intptr_t)f);
}

int w16__lclose(short h) W16(_lclose);
int w16__lclose(short h)
{
    HFILE f = (HFILE)(intptr_t)w16_handle((unsigned short)h);
    w16_forget((unsigned short)h);
    return _lclose(f);
}

long w16__llseek(short h, long off, int origin) W16(_llseek);
long w16__llseek(short h, long off, int origin)
{
    return _llseek((HFILE)(intptr_t)w16_handle((unsigned short)h), off, origin);
}

long w16__hread(short h, void *buf, long n) W16(_hread);
long w16__hread(short h, void *buf, long n)
{
    return _hread((HFILE)(intptr_t)w16_handle((unsigned short)h), buf, n);
}

long w16__hwrite(short h, void *buf, long n) W16(_hwrite);
long w16__hwrite(short h, void *buf, long n)
{
    return _hwrite((HFILE)(intptr_t)w16_handle((unsigned short)h), (LPCCH)buf, n);
}
