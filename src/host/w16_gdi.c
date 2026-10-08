/* GDI, Win16 on Win32 (roadmap W4): device contexts and objects as 16-bit tokens, LOGFONT,
   SIZE and POINT in their 16-bit layouts, MoveTo's packed return. Palettes are passed on as
   they are; on a display that is not 8-bit they do not reach the screen, which is the
   WinG layer's job (w16_wing.c redraws through the DIB's colour table). */
#include <string.h>
#include "w16.h"

#define TOK(h) w16_token((HANDLE)(h))
#define DC(t) W16_HDC(t)
#define OBJ(t) ((HGDIOBJ)w16_handle(t))

typedef struct {                          /* Win16 LOGFONT (include/gen/w16types.h) */
    int16_t lfHeight, lfWidth, lfEscapement, lfOrientation, lfWeight;
    unsigned char lfItalic, lfUnderline, lfStrikeOut, lfCharSet, lfOutPrecision, lfClipPrecision,
        lfQuality, lfPitchAndFamily;
    char lfFaceName[32];
} __attribute__((packed)) LOGFONT16;

typedef struct { int16_t cx, cy; } SIZE16;

typedef struct {                          /* TEXTMETRIC as the game's files declare it */
    int tmHeight, tmAscent, tmDescent, tmInternalLeading, tmExternalLeading, tmAveCharWidth, tmMaxCharWidth,
        tmWeight;
    unsigned char tmItalic, tmUnderlined, tmStruckOut, tmFirstChar, tmLastChar, tmDefaultChar, tmBreakChar,
        tmPitchAndFamily, tmCharSet;
    int tmOverhang, tmDigitizedAspectX, tmDigitizedAspectY;
} TEXTMETRIC_G;

/* ------------------------------------------------------------------ DCs and objects */

unsigned w16_CreateCompatibleDC(unsigned dc) W16(CreateCompatibleDC);
unsigned w16_CreateCompatibleDC(unsigned dc) { return TOK(CreateCompatibleDC(DC(dc))); }

unsigned w16_CreateDC(const char *drv, const char *dev, const char *out, void *init) W16(CreateDC);
unsigned w16_CreateDC(const char *drv, const char *dev, const char *out, void *init)
{
    (void)init;
    return TOK(CreateDCA(drv, dev, out, 0));
}

int w16_DeleteDC(unsigned dc) W16(DeleteDC);
int w16_DeleteDC(unsigned dc)
{
    int r = DeleteDC(DC(dc));
    w16_forget(dc);
    return r;
}

int w16_DeleteObject(unsigned o) W16(DeleteObject);
int w16_DeleteObject(unsigned o)
{
    int r = DeleteObject(OBJ(o));
    if (r)
        w16_forget(o);
    return r;
}

unsigned w16_SelectObject(unsigned dc, unsigned o) W16(SelectObject);
unsigned w16_SelectObject(unsigned dc, unsigned o) { return TOK(SelectObject(DC(dc), OBJ(o))); }

unsigned w16_GetStockObject(int i) W16(GetStockObject);
unsigned w16_GetStockObject(int i) { return TOK(GetStockObject(i)); }

unsigned w16_CreatePen(int style, int width, unsigned long color) W16(CreatePen);
unsigned w16_CreatePen(int style, int width, unsigned long color) { return TOK(CreatePen(style, width, color)); }

unsigned w16_CreateSolidBrush(unsigned long color) W16(CreateSolidBrush);
unsigned w16_CreateSolidBrush(unsigned long color) { return TOK(CreateSolidBrush(color)); }

unsigned w16_CreateBitmap(int w, int h, unsigned planes, unsigned bpp, void *bits) W16(CreateBitmap);
unsigned w16_CreateBitmap(int w, int h, unsigned planes, unsigned bpp, void *bits)
{
    if ((long long)w * h * bpp >= 128ll << 20 || w < 0 || h < 0)
        w16_report("huge CreateBitmap(%d x %d x %u) from %p", w, h, bpp, __builtin_return_address(0));
    return TOK(CreateBitmap(w, h, planes, bpp, bits));
}

unsigned w16_CreateCompatibleBitmap(unsigned dc, int w, int h) W16(CreateCompatibleBitmap);
unsigned w16_CreateCompatibleBitmap(unsigned dc, int w, int h)
{
    if ((long long)w * h >= 16ll << 20 || w < 0 || h < 0)
        w16_report("huge CreateCompatibleBitmap(%d x %d) from %p", w, h, __builtin_return_address(0));
    return TOK(CreateCompatibleBitmap(DC(dc), w, h));
}

int w16_GetBitmapDimensionEx(unsigned b, SIZE16 *s) W16(GetBitmapDimensionEx);
int w16_GetBitmapDimensionEx(unsigned b, SIZE16 *s)
{
    SIZE z;
    int r = GetBitmapDimensionEx((HBITMAP)OBJ(b), &z);
    s->cx = (int16_t)z.cx; s->cy = (int16_t)z.cy;
    return r;
}

int w16_GetDeviceCaps(unsigned dc, int i) W16(GetDeviceCaps);
int w16_GetDeviceCaps(unsigned dc, int i) { return w16_device_caps(DC(dc), i); }

int w16_BitBlt(unsigned d, int x, int y, int w, int h, unsigned s, int sx, int sy, unsigned long rop) W16(BitBlt);
int w16_BitBlt(unsigned d, int x, int y, int w, int h, unsigned s, int sx, int sy, unsigned long rop)
{
    return BitBlt(DC(d), (int16_t)x, (int16_t)y, (int16_t)w, (int16_t)h, DC(s), (int16_t)sx, (int16_t)sy, rop);
}

/* ------------------------------------------------------------------ lines */

unsigned long w16_MoveTo(unsigned dc, int x, int y) W16(MoveTo);
unsigned long w16_MoveTo(unsigned dc, int x, int y)
{
    POINT p;
    MoveToEx(DC(dc), (int16_t)x, (int16_t)y, &p);
    return MAKELONG((int16_t)p.x, (int16_t)p.y);
}

int w16_MoveToEx(unsigned dc, int x, int y, POINT16 *old) W16(MoveToEx);
int w16_MoveToEx(unsigned dc, int x, int y, POINT16 *old)
{
    POINT p;
    int r = MoveToEx(DC(dc), (int16_t)x, (int16_t)y, &p);
    if (old) {
        old->x = (int16_t)p.x; old->y = (int16_t)p.y;
    }
    return r;
}

int w16_LineTo(unsigned dc, int x, int y) W16(LineTo);
int w16_LineTo(unsigned dc, int x, int y) { return LineTo(DC(dc), (int16_t)x, (int16_t)y); }

/* ------------------------------------------------------------------ text */

unsigned long w16_SetBkColor(unsigned dc, unsigned long c) W16(SetBkColor);
unsigned long w16_SetBkColor(unsigned dc, unsigned long c) { return SetBkColor(DC(dc), c); }

int w16_SetBkMode(unsigned dc, int m) W16(SetBkMode);
int w16_SetBkMode(unsigned dc, int m) { return SetBkMode(DC(dc), m); }

unsigned long w16_SetTextColor(unsigned dc, unsigned long c) W16(SetTextColor);
unsigned long w16_SetTextColor(unsigned dc, unsigned long c) { return SetTextColor(DC(dc), c); }

unsigned w16_SetTextAlign(unsigned dc, unsigned a) W16(SetTextAlign);
unsigned w16_SetTextAlign(unsigned dc, unsigned a) { return SetTextAlign(DC(dc), a); }

unsigned w16_GetTextAlign(unsigned dc) W16(GetTextAlign);
unsigned w16_GetTextAlign(unsigned dc) { return GetTextAlign(DC(dc)); }

int w16_GetTextExtentPoint(unsigned dc, const char *s, int n, SIZE16 *sz) W16(GetTextExtentPoint);
int w16_GetTextExtentPoint(unsigned dc, const char *s, int n, SIZE16 *sz)
{
    SIZE z;
    int r = GetTextExtentPoint32A(DC(dc), s, (int16_t)n, &z);
    sz->cx = (int16_t)z.cx; sz->cy = (int16_t)z.cy;
    return r;
}

int w16_GetTextMetrics(unsigned dc, TEXTMETRIC_G *g) W16(GetTextMetrics);
int w16_GetTextMetrics(unsigned dc, TEXTMETRIC_G *g)
{
    TEXTMETRICA t;
    int r = GetTextMetricsA(DC(dc), &t);
    g->tmHeight = t.tmHeight; g->tmAscent = t.tmAscent; g->tmDescent = t.tmDescent;
    g->tmInternalLeading = t.tmInternalLeading; g->tmExternalLeading = t.tmExternalLeading;
    g->tmAveCharWidth = t.tmAveCharWidth; g->tmMaxCharWidth = t.tmMaxCharWidth; g->tmWeight = t.tmWeight;
    g->tmItalic = t.tmItalic; g->tmUnderlined = t.tmUnderlined; g->tmStruckOut = t.tmStruckOut;
    g->tmFirstChar = t.tmFirstChar; g->tmLastChar = t.tmLastChar; g->tmDefaultChar = t.tmDefaultChar;
    g->tmBreakChar = t.tmBreakChar; g->tmPitchAndFamily = t.tmPitchAndFamily; g->tmCharSet = t.tmCharSet;
    g->tmOverhang = t.tmOverhang; g->tmDigitizedAspectX = t.tmDigitizedAspectX;
    g->tmDigitizedAspectY = t.tmDigitizedAspectY;
    return r;
}

static void font_in(LOGFONTA *d, const LOGFONT16 *s)
{
    memset(d, 0, sizeof *d);
    d->lfHeight = s->lfHeight; d->lfWidth = s->lfWidth; d->lfEscapement = s->lfEscapement;
    d->lfOrientation = s->lfOrientation; d->lfWeight = s->lfWeight; d->lfItalic = s->lfItalic;
    d->lfUnderline = s->lfUnderline; d->lfStrikeOut = s->lfStrikeOut; d->lfCharSet = s->lfCharSet;
    d->lfOutPrecision = s->lfOutPrecision; d->lfClipPrecision = s->lfClipPrecision; d->lfQuality = s->lfQuality;
    d->lfPitchAndFamily = s->lfPitchAndFamily;
    memcpy(d->lfFaceName, s->lfFaceName, 32);
}

static void font_out(LOGFONT16 *d, const LOGFONTA *s)
{
    d->lfHeight = (int16_t)s->lfHeight; d->lfWidth = (int16_t)s->lfWidth;
    d->lfEscapement = (int16_t)s->lfEscapement; d->lfOrientation = (int16_t)s->lfOrientation;
    d->lfWeight = (int16_t)s->lfWeight; d->lfItalic = s->lfItalic; d->lfUnderline = s->lfUnderline;
    d->lfStrikeOut = s->lfStrikeOut; d->lfCharSet = s->lfCharSet; d->lfOutPrecision = s->lfOutPrecision;
    d->lfClipPrecision = s->lfClipPrecision; d->lfQuality = s->lfQuality; d->lfPitchAndFamily = s->lfPitchAndFamily;
    memcpy(d->lfFaceName, s->lfFaceName, 32);
}

unsigned w16_CreateFontIndirect(LOGFONT16 *f) W16(CreateFontIndirect);
unsigned w16_CreateFontIndirect(LOGFONT16 *f)
{
    LOGFONTA lf;
    font_in(&lf, f);
    return TOK(CreateFontIndirectA(&lf));
}

int w16_AddFontResource(const char *name) W16(AddFontResource);
int w16_AddFontResource(const char *name) { return AddFontResourceA(name); }

int w16_RemoveFontResource(char *name) W16(RemoveFontResource);
int w16_RemoveFontResource(char *name) { return RemoveFontResourceA(name); }

/* EnumFontFamilies: the game's procedure is Win16-shaped and cdecl; Windows calls this one */
typedef int (*GAMEFONTPROC)(LOGFONT16 *, void *, int, long);
struct enum_ctx { GAMEFONTPROC proc; long lparam; };

static int CALLBACK font_thunk(const LOGFONTA *lf, const TEXTMETRICA *tm, DWORD type, LPARAM lp)
{
    struct enum_ctx *c = (struct enum_ctx *)lp;
    LOGFONT16 f;
    (void)tm;
    font_out(&f, lf);
    return c->proc(&f, 0, (int)type, c->lparam);
}

int w16_EnumFontFamilies(unsigned dc, const char *family, GAMEFONTPROC proc, void *lparam) W16(EnumFontFamilies);
int w16_EnumFontFamilies(unsigned dc, const char *family, GAMEFONTPROC proc, void *lparam)
{
    struct enum_ctx c = { proc, (long)(intptr_t)lparam };
    return EnumFontFamiliesA(DC(dc), family, font_thunk, (LPARAM)&c);
}

/* ------------------------------------------------------------------ palettes */

int w16_CreatePalette(void *lp) W16(CreatePalette);
int w16_CreatePalette(void *lp) { return (int)TOK(CreatePalette((LOGPALETTE *)lp)); }

unsigned w16_SelectPalette(unsigned dc, unsigned p, int bg) W16(SelectPalette);
unsigned w16_SelectPalette(unsigned dc, unsigned p, int bg) { return TOK(SelectPalette(DC(dc), (HPALETTE)OBJ(p), bg)); }

unsigned w16_RealizePalette(unsigned dc) W16(RealizePalette);
unsigned w16_RealizePalette(unsigned dc)
{
    HPALETTE p = (HPALETTE)GetCurrentObject(DC(dc), OBJ_PAL);
    unsigned r = RealizePalette(DC(dc));
    if (w16_palette_device() && p) {
        w16_realize(p);
        return 236;                       /* entries mapped, as a palette device answers */
    }
    return r;
}

void w16_AnimatePalette(unsigned p, unsigned start, unsigned n, void *e) W16(AnimatePalette);
void w16_AnimatePalette(unsigned p, unsigned start, unsigned n, void *e)
{
    AnimatePalette((HPALETTE)OBJ(p), start, n, (PALETTEENTRY *)e);
    if (w16_palette_device())
        w16_animate(start, n, (const PALETTEENTRY *)e);
}

unsigned w16_GetPaletteEntries(unsigned p, unsigned start, unsigned n, PALETTEENTRY *e) W16(GetPaletteEntries);
unsigned w16_GetPaletteEntries(unsigned p, unsigned start, unsigned n, PALETTEENTRY *e)
{
    return GetPaletteEntries((HPALETTE)OBJ(p), start, n, e);
}

void w16_SetPaletteEntries(unsigned p, unsigned start, unsigned n, void *e) W16(SetPaletteEntries);
void w16_SetPaletteEntries(unsigned p, unsigned start, unsigned n, void *e)
{
    SetPaletteEntries((HPALETTE)OBJ(p), start, n, (PALETTEENTRY *)e);
}

int w16_GetSystemPaletteEntries(unsigned dc, unsigned start, unsigned n, void *e) W16(GetSystemPaletteEntries);
int w16_GetSystemPaletteEntries(unsigned dc, unsigned start, unsigned n, void *e)
{
    if (w16_palette_device())
        return (int)w16_system_palette(start, n, (PALETTEENTRY *)e);
    return GetSystemPaletteEntries(DC(dc), start, n, (PALETTEENTRY *)e);
}
