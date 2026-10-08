/* COMMDLG: the open and save file dialogs, Win16 on Win32 (roadmap W4). The game's OPENFILENAME
   is its own structure in the port's layout (the field order of Win32's, handles as tokens);
   it is copied into Win32's, the dialog run, and the results copied back. */
#include <string.h>
#include "w16.h"
#include <commdlg.h>

typedef struct {
    unsigned long lStructSize; unsigned hwndOwner; unsigned hInstance;
    const char *lpstrFilter; char *lpstrCustomFilter; unsigned long nMaxCustFilter;
    unsigned long nFilterIndex; char *lpstrFile; unsigned long nMaxFile;
    char *lpstrFileTitle; unsigned long nMaxFileTitle; const char *lpstrInitialDir;
    const char *lpstrTitle; unsigned long Flags; unsigned nFileOffset; unsigned nFileExtension;
    const char *lpstrDefExt; unsigned long lCustData; void *lpfnHook; const char *lpTemplateName;
} OPENFILENAME_G;

static void in(OPENFILENAMEA *o, const OPENFILENAME_G *g)
{
    memset(o, 0, sizeof *o);
    o->lStructSize = sizeof *o;
    o->hwndOwner = W16_HWND(g->hwndOwner);
    o->lpstrFilter = g->lpstrFilter;
    o->lpstrCustomFilter = g->lpstrCustomFilter;
    o->nMaxCustFilter = g->nMaxCustFilter;
    o->nFilterIndex = g->nFilterIndex;
    o->lpstrFile = g->lpstrFile;
    o->nMaxFile = g->nMaxFile;
    o->lpstrFileTitle = g->lpstrFileTitle;
    o->nMaxFileTitle = g->nMaxFileTitle;
    o->lpstrInitialDir = g->lpstrInitialDir;
    o->lpstrTitle = g->lpstrTitle;
    /* no hook or template from the game's module: Win32 cannot use them */
    o->Flags = g->Flags & ~(OFN_ENABLEHOOK | OFN_ENABLETEMPLATE | OFN_ENABLETEMPLATEHANDLE);
    o->lpstrDefExt = g->lpstrDefExt;
}

static void out(OPENFILENAME_G *g, const OPENFILENAMEA *o)
{
    g->nFilterIndex = o->nFilterIndex;
    g->nFileOffset = o->nFileOffset;
    g->nFileExtension = o->nFileExtension;
    g->Flags = (g->Flags & (OFN_ENABLEHOOK | OFN_ENABLETEMPLATE | OFN_ENABLETEMPLATEHANDLE)) | o->Flags;
}

char w16_GetOpenFileName(OPENFILENAME_G *g) W16(GetOpenFileName);
char w16_GetOpenFileName(OPENFILENAME_G *g)
{
    OPENFILENAMEA o;
    BOOL r;
    in(&o, g);
    r = GetOpenFileNameA(&o);
    out(g, &o);
    return (char)r;
}

char w16_GetSaveFileName(OPENFILENAME_G *g) W16(GetSaveFileName);
char w16_GetSaveFileName(OPENFILENAME_G *g)
{
    OPENFILENAMEA o;
    BOOL r;
    in(&o, g);
    r = GetSaveFileNameA(&o);
    out(g, &o);
    return (char)r;
}

unsigned long w16_CommDlgExtendedError(void) W16(CommDlgExtendedError);
unsigned long w16_CommDlgExtendedError(void) { return CommDlgExtendedError(); }
