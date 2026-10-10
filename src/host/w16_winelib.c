/* The Linux build's start (roadmap W8: Winelib, tools/undefined.py with PORT_TOOLCHAIN=winegcc).
 *
 * The game needs a deep stack: its functions keep their records and buffers in locals, and the
 * MinGW build links a 64 MB one (-Wl,--stack). A Winelib program's main thread gets Wine's
 * default and winebuild has no option for it, so the link wraps WinMain (-Wl,--wrap=WinMain):
 * the startup's call lands here, and the game's WinMain runs on a thread of its own with the
 * same 64 MB. Its windows and message queue are that thread's; this one only waits.
 * Nothing here exists in the Windows build.
 */
#ifdef __WINE__
#include "w16.h"

extern int __stdcall __real_WinMain(unsigned hinst, unsigned hprev, char *cmdline, int show);

static struct { unsigned hinst, hprev; char *cmdline; int show, ret; } args;

static DWORD WINAPI game_thread(void *unused)
{
    (void)unused;
    args.ret = __real_WinMain(args.hinst, args.hprev, args.cmdline, args.show);
    return 0;
}

void w16_crash_install(void);
void port_load_initial_data(void);
extern int w16_initial_loaded;

/* the game's static constructors (Font g_font16(1, 0x10), LogWindow g_error_log(g_log_name)),
   moved out of .init_array by tools/undefined.py; the linker names the section's ends */
typedef void (*ctor_fn)(void);
extern ctor_fn __start_port_ctors[] __attribute__((weak));
extern ctor_fn __stop_port_ctors[] __attribute__((weak));

int __stdcall __wrap_WinMain(unsigned hinst, unsigned hprev, char *cmdline, int show)
{
    HANDLE t;
    ctor_fn *c;
    w16_crash_install();                 /* not from a constructor here (w16_crash.c) */
    /* the Windows build's order: the crash report, the initial data (constructor 101), then the game's constructors */
    port_load_initial_data();
    for (c = __start_port_ctors; c && c < __stop_port_ctors; c++)
        (*c)();
    w16_report("winelib start: initial data %d globals, %d constructors", w16_initial_loaded,
               (int)(__stop_port_ctors - __start_port_ctors));
    args.hinst = hinst;
    args.hprev = hprev;
    args.cmdline = cmdline;
    args.show = show;
    t = CreateThread(0, 64u << 20, game_thread, 0, STACK_SIZE_PARAM_IS_A_RESERVATION, 0);
    if (!t)
        return __real_WinMain(hinst, hprev, cmdline, show);
    WaitForSingleObject(t, INFINITE);
    CloseHandle(t);
    return args.ret;
}
#endif
