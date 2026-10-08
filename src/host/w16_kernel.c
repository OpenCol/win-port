/* KERNEL: memory handles and the rest of the small kernel calls, Win16 on Win32 (roadmap W4).
   Global and local memory handles are tokens over Win32's global heap; a huge pointer is a
   flat one; procedure instances are the procedures themselves. */
#include <string.h>
#include "w16.h"

#define TOK(h) w16_token((HANDLE)(h))

unsigned w16_GlobalAlloc(unsigned flags, unsigned long size) W16(GlobalAlloc);
unsigned w16_GlobalAlloc(unsigned flags, unsigned long size)
{
    HGLOBAL h = GlobalAlloc(flags & (GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_FIXED), size ? size : 1);
    return TOK(h);
}

unsigned w16_GlobalFree(unsigned h) W16(GlobalFree);
unsigned w16_GlobalFree(unsigned h)
{
    HGLOBAL r = GlobalFree((HGLOBAL)w16_handle(h));
    if (!r)
        w16_forget(h);
    return r ? h : 0;
}

void *w16_GlobalLock(unsigned h) W16(GlobalLock);
void *w16_GlobalLock(unsigned h) { return h ? GlobalLock((HGLOBAL)w16_handle(h)) : 0; }

int w16_GlobalUnlock(unsigned h) W16(GlobalUnlock);
int w16_GlobalUnlock(unsigned h) { return GlobalUnlock((HGLOBAL)w16_handle(h)); }

unsigned long w16_GlobalSize(unsigned h) W16(GlobalSize);
unsigned long w16_GlobalSize(unsigned h) { return (unsigned long)GlobalSize((HGLOBAL)w16_handle(h)); }

unsigned w16_LocalAlloc(unsigned flags, unsigned size) W16(LocalAlloc);
unsigned w16_LocalAlloc(unsigned flags, unsigned size)
{
    return TOK(LocalAlloc(flags & (LMEM_MOVEABLE | LMEM_ZEROINIT), size ? size : 1));
}

unsigned w16_LocalFree(unsigned h) W16(LocalFree);
unsigned w16_LocalFree(unsigned h)
{
    HLOCAL r = LocalFree((HLOCAL)w16_handle(h));
    if (!r)
        w16_forget(h);
    return r ? h : 0;
}

char *w16_LocalLock(unsigned h) W16(LocalLock);
char *w16_LocalLock(unsigned h) { return h ? (char *)LocalLock((HLOCAL)w16_handle(h)) : 0; }

int w16_LocalUnlock(unsigned h) W16(LocalUnlock);
int w16_LocalUnlock(unsigned h) { return LocalUnlock((HLOCAL)w16_handle(h)); }

/* the game asks how much memory there is before loading art: plenty */
unsigned long w16_GetFreeSpace(unsigned flags) W16(GetFreeSpace);
unsigned long w16_GetFreeSpace(unsigned flags)
{
    (void)flags;
    return 16ul << 20;
}

unsigned w16_GetFreeSystemResources(unsigned which) W16(GetFreeSystemResources);
unsigned w16_GetFreeSystemResources(unsigned which)
{
    (void)which;
    return 80;                            /* percent free, as a roomy Windows 3.1 said */
}

typedef void (*FARPROC_G)(void);

FARPROC_G w16_MakeProcInstance(FARPROC_G p, unsigned hinst) W16(MakeProcInstance);
FARPROC_G w16_MakeProcInstance(FARPROC_G p, unsigned hinst)
{
    (void)hinst;
    return p;                             /* no data-segment thunks in a flat program */
}

void w16_FreeProcInstance(FARPROC_G p) W16(FreeProcInstance);
void w16_FreeProcInstance(FARPROC_G p) { (void)p; }

void w16_hmemcpy(void *d, void *s, long n) W16(hmemcpy);
void w16_hmemcpy(void *d, void *s, long n) { memmove(d, s, (size_t)n); }

void w16_OutputDebugString(const char *s) W16(OutputDebugString);
void w16_OutputDebugString(const char *s)
{
    OutputDebugStringA(s);
    w16_trace("debug: %s", s);
}
