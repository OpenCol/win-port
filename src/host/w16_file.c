/* KERNEL's file functions (roadmap W4). Win32 keeps OpenFile, _lclose, _llseek, _hread and
   _hwrite, with Win16's OFSTRUCT layout and OF_* flags; the file handle the game keeps in a
   `short` is a 16-bit token. */
#include "w16.h"

short w16_OpenFile(const char *name, void *ofs, unsigned mode) W16(OpenFile);
short w16_OpenFile(const char *name, void *ofs, unsigned mode)
{
    HFILE f = OpenFile(name, (OFSTRUCT *)ofs, mode);
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
