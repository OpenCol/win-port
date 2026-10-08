/* The autoplay lockstep (roadmap W6, tests/lockstep.py): the port playing a saved game by itself
 * the way the oracle's recorder makes the original play it, and dumping its state at the same
 * moments.
 *
 * Off unless $PORT_LOCKSTEP names a directory holding lockstep.txt (written by tests/lockstep.py
 * from the oracle's recording):
 *     save PATH       the file the Load dialog is answered with
 *     key VK          a key, posted once the game sits idle (the menu keys to Load; a session's)
 *     cmd N           a menu command (WM_COMMAND N to the main window), once the game sits idle
 *     snap NAME       the screen as it is, to NAME.bmp in this directory ($PORT_SNAPSHOT set)
 *     grab NAME       the windows as Wine shows them (after the palette, GDI text, menus), to NAME.bmp
 *     click X Y       a left click at (X, Y) of a grab's image, to the window under it
 *     wait N          N more idle rounds before the next
 *     exit            the end of a session: the program stops
 *                     (key, cmd, snap, wait and exit are one script, run in order)
 *     noautoplay      the player stays human: no all-AI stores (a UI session, roadmap W7)
 *     saveas PATH     the file the Save dialog is answered with
 *     nudge N         a Return the oracle posted at its checkpoint N (a modal message)
 *     srand N         the seeds the original's srand got, in order
 *     l17 N           the seeds its assembly generator (L17_01f0) got, in order
 *     turns N         stop at the checkpoint N turns past the first one in a game (as the recorder)
 * While it runs:
 *   - time is the emulator's: GetTickCount is a counter that moves as winsim's does (17 a read,
 *     5 an empty PeekMessage, 1 a message), and timers fire from it, not from the wall clock;
 *   - the seeds the game draws from that clock are replaced by the oracle's, one by one;
 *   - the host's keyboard and mouse never reach the game: only the script's keys do;
 *   - MCI and sndPlaySound do nothing and answer 1, as winsim's defaults do (no song ends at a
 *     moment the host decides);
 *   - at every write of GameBrains.step (w16_lockstep_step, called after each one in the game),
 *     the cheat menu's all-AI stores are made, and when (turn, step) has changed the state is
 *     appended to native.cp: every global of src/gen's table as the port holds it, the four
 *     map planes, the two generators' states.
 * native.log records what happened (keys, seeds, checkpoints).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "w16.h"

struct PortInitGlobal { void *addr; unsigned short seg, off, count, w16, wn, kind; };
extern const struct PortInitGlobal port_init_globals[];
extern const unsigned port_init_global_count;
extern unsigned long w16_borland_seed(void);
extern uint32_t g_dgroup_3f00_seed;
extern long (*port_l17_seed_filter)(long);
void w16_trace_mark(const char *text);
int w16_snapshot_write(const char *path);
int w16_grab_write(const char *path);
HWND w16_grab_click(int x, int y, POINT *client);

#define SEG20 20
#define DGROUP 34
#define IDLE_POLLS 400                   /* empty PeekMessages in a row: the game waits for input */

static int active = -1;
static char dir[MAX_PATH], save[MAX_PATH];
static FILE *log_f, *cp_f;
static unsigned long ticks;
static struct { char kind; unsigned long v; char text[64]; } script[512];
static unsigned nscript, iscript, waitn;
static int noautoplay;
static HWND click_h;
static LPARAM click_lp;
static int click_left;
static char saveas[MAX_PATH];
static unsigned nudges[256], nnudges, inudge;
static unsigned long srands[4096], nsrand, isrand;
static uint32_t l17s[4096];
static unsigned long nl17, il17;
static unsigned idle, checkpoints, turns;
static int start_turn = -32768;
static int last_turn = -32768, last_step = -32768;
static struct { HWND h; unsigned id, ms; unsigned long due; } timers[32];

static void *global_at(unsigned seg, unsigned off)
{
    unsigned i;
    for (i = 0; i < port_init_global_count; i++)
        if (port_init_globals[i].seg == seg && port_init_globals[i].off == off)
            return port_init_globals[i].addr;
    return 0;
}

static long l17_filter(long seed);

static void init(void)
{
    const char *d = getenv("PORT_LOCKSTEP");
    char path[MAX_PATH + 32], line[MAX_PATH + 32];
    FILE *f;
    active = 0;
    if (!d || !*d)
        return;
    snprintf(dir, sizeof dir, "%s", d);
    snprintf(path, sizeof path, "%s\\lockstep.txt", dir);
    if (!(f = fopen(path, "r")))
        return;
    while (fgets(line, sizeof line, f)) {
        char *v = strchr(line, ' ');
        if (!v) {
            line[strcspn(line, "\r\n")] = 0;
            v = line + strlen(line);         /* a bare word: noautoplay, exit */
        } else
            *v++ = 0;
        v[strcspn(v, "\r\n")] = 0;
        if (!strcmp(line, "save"))
            snprintf(save, sizeof save, "%s", v);
        else if (!strcmp(line, "noautoplay"))
            noautoplay = 1;
        else if (!strcmp(line, "saveas"))
            snprintf(saveas, sizeof saveas, "%s", v);
        else if ((!strcmp(line, "key") || !strcmp(line, "cmd") || !strcmp(line, "snap") || !strcmp(line, "wait")
                  || !strcmp(line, "grab") || !strcmp(line, "click")
                  || !strcmp(line, "exit")) && nscript < 512) {
            script[nscript].kind = strcmp(line, "click") ? line[0] : 'm';
            script[nscript].v = strtoul(v, 0, 0);
            snprintf(script[nscript].text, sizeof script[nscript].text, "%s", v);
            nscript++;
        }
        else if (!strcmp(line, "nudge") && nnudges < 256)
            nudges[nnudges++] = (unsigned)strtoul(v, 0, 0);
        else if (!strcmp(line, "srand") && nsrand < 4096)
            srands[nsrand++] = strtoul(v, 0, 0);
        else if (!strcmp(line, "turns"))
            turns = (unsigned)strtoul(v, 0, 0);
        else if (!strcmp(line, "l17") && nl17 < 4096)
            l17s[nl17++] = (uint32_t)strtoul(v, 0, 0);
    }
    fclose(f);
    snprintf(path, sizeof path, "%s\\native.log", dir);
    log_f = fopen(path, "w");
    snprintf(path, sizeof path, "%s\\native.cp", dir);
    cp_f = fopen(path, "wb");
    port_l17_seed_filter = l17_filter;
    active = 1;
    fprintf(log_f, "lockstep: save %s, %u script items, %u nudges, %lu srand and %lu l17 seeds%s\n", save, nscript,
            nnudges, nsrand, nl17, noautoplay ? ", no autoplay" : "");
    fflush(log_f);
}

int w16_lockstep(void)
{
    if (active < 0)
        init();
    return active;
}

const char *w16_lockstep_save(void)
{
    return w16_lockstep() && *save ? save : 0;
}

const char *w16_lockstep_saveas(void)
{
    return w16_lockstep() && *saveas ? saveas : 0;
}

/* ------------------------------------------------------------------ time */

unsigned long w16_lockstep_ticks(void)
{
    ticks += 17;
    return ticks;
}

unsigned w16_lockstep_set_timer(HWND h, unsigned id, unsigned ms)
{
    int i, free_slot = -1;
    for (i = 0; i < 32; i++) {
        if (timers[i].h == h && timers[i].id == id && timers[i].ms)
            break;
        if (free_slot < 0 && !timers[i].ms)
            free_slot = i;
    }
    if (i == 32)
        i = free_slot;
    if (i < 0)
        return 0;
    timers[i].h = h;
    timers[i].id = id;
    timers[i].ms = ms ? ms : 1;
    timers[i].due = ticks + timers[i].ms;
    return id ? id : (unsigned)(i + 1);
}

int w16_lockstep_kill_timer(HWND h, unsigned id)
{
    int i;
    for (i = 0; i < 32; i++)
        if (timers[i].h == h && timers[i].id == id && timers[i].ms) {
            timers[i].ms = 0;
            return 1;
        }
    return 0;
}

static void post_key(unsigned vk)
{
    HWND h = GetFocus();
    if (!h)
        h = GetActiveWindow();
    /* as the oracle posts a key (dyn_trace's step): down, a character, up */
    PostMessageA(h, WM_KEYDOWN, vk, 1);
    PostMessageA(h, WM_CHAR, vk, 1);
    PostMessageA(h, WM_KEYUP, vk, 0xC0000001);
    fprintf(log_f, "key %#x to %p at tick %lu, checkpoint %u\n", vk, (void *)h, ticks, checkpoints);
    fflush(log_f);
}

/* PeekMessage found nothing: time passes, due timers fire, and a game waiting for input gets
   the next key */
void w16_lockstep_idle(void)
{
    int i;
    ticks += 5;
    for (i = 0; i < 32; i++)
        if (timers[i].ms && ticks >= timers[i].due) {
            timers[i].due = ticks + timers[i].ms;
            PostMessageA(timers[i].h, WM_TIMER, timers[i].id, 0);
        }
    if (++idle < IDLE_POLLS)
        return;
    idle = 0;
    if (waitn) {
        waitn--;
        return;
    }
    if (iscript < nscript) {
        unsigned k = iscript++;
        switch (script[k].kind) {
        case 'k':
            post_key((unsigned)script[k].v);
            break;
        case 'c': {
            HWND h = FindWindowA(0, "Colonization for Windows");
            if (!h)
                h = GetActiveWindow();
            PostMessageA(h, WM_COMMAND, (WPARAM)script[k].v, 0);
            fprintf(log_f, "cmd %lu to %p at tick %lu\n", script[k].v, (void *)h, ticks);
            break;
        }
        case 's': {
            char path[MAX_PATH + 80];
            snprintf(path, sizeof path, "%s\\%s.bmp", dir, script[k].text);
            fprintf(log_f, "snap %s: %s\n", script[k].text, w16_snapshot_write(path) ? "written" : "no shadow screen");
            break;
        }
        case 'm': {
            char *e;
            int x = (int)strtol(script[k].text, &e, 0), y = (int)strtol(e, 0, 0);
            POINT p;
            click_left = 3;
            click_h = w16_grab_click(x, y, &p);
            click_lp = MAKELPARAM(p.x, p.y);
            fprintf(log_f, "click %d %d to %p at tick %lu\n", x, y, (void *)click_h, ticks);
            break;
        }
        case 'g': {
            char path[MAX_PATH + 80];
            snprintf(path, sizeof path, "%s\\%s.bmp", dir, script[k].text);
            fprintf(log_f, "grab %s: %s\n", script[k].text, w16_grab_write(path) ? "written" : "no window");
            break;
        }
        case 'w':
            waitn = (unsigned)script[k].v;
            break;
        case 'e':
            fprintf(log_f, "session done\n");
            fclose(log_f);
            if (cp_f)
                fclose(cp_f);
            ExitProcess(0);
        }
        fflush(log_f);
    } else if (inudge < nnudges && checkpoints >= nudges[inudge]) {
        inudge++;
        post_key(VK_RETURN);
    }
}

/* a message PeekMessage would hand the game: input only if the script posted it */
int w16_lockstep_input(const MSG *m)
{
    int key = m->message >= WM_KEYFIRST && m->message <= WM_KEYLAST;
    int mouse = (m->message >= WM_MOUSEFIRST && m->message <= WM_MOUSELAST) ||
                (m->message >= WM_NCMOUSEMOVE && m->message <= WM_NCMBUTTONDBLCLK);
    if (mouse)                           /* ours: a script's click, its three messages */
        return click_left && m->hwnd == click_h && m->lParam == click_lp && click_left--;
    if (key)                             /* ours: lParam 1 (down, char) or 0xC0000001 (up) */
        return m->lParam == 1 || (unsigned long)m->lParam == 0xC0000001ul;
    return 1;
}

void w16_lockstep_message(const MSG *m)
{
    ticks += 1;
    /* in a game, a timer or a paint is the game waiting still, not input it is busy with: a human
       turn's cursor blinks on a timer, and that must not hold a session's next step back. Before
       one (the intro, the menus) they count as busy, as they always did: the intro runs on its
       timers, and a key then would skip it instead of reaching the menu */
    unsigned char *game = global_at(SEG20, 0x7782);
    int in_game = game && *(int16_t *)(game + 66);
    if (!in_game || (m->message != WM_TIMER && m->message != WM_PAINT))
        idle = 0;
}

/* ------------------------------------------------------------------ the generators' seeds */

unsigned w16_lockstep_srand(unsigned seed)
{
    if (isrand < nsrand) {
        fprintf(log_f, "srand %u -> %lu (%lu)\n", seed, srands[isrand], isrand);
        return (unsigned)srands[isrand++];
    }
    fprintf(log_f, "srand %u: past the oracle's %lu seeds\n", seed, nsrand);
    isrand++;
    return seed;
}

static long l17_filter(long seed)
{
    if (il17 < nl17) {
        fprintf(log_f, "l17 %ld -> %lu (%lu)\n", seed, (unsigned long)l17s[il17], il17);
        return (long)l17s[il17++];
    }
    fprintf(log_f, "l17 %ld: past the oracle's %lu seeds\n", seed, nl17);
    il17++;
    return seed;
}

/* ------------------------------------------------------------------ the checkpoint */

static void wr32(uint32_t v)
{
    fwrite(&v, 4, 1, cp_f);
}

static void dump(int turn, int step)
{
    unsigned i;
    uint32_t n = 0;
    void **planes[4];
    fwrite("CP", 1, 2, cp_f);
    wr32(checkpoints);
    wr32((uint32_t)(uint16_t)turn);
    wr32((uint32_t)(uint16_t)step);
    wr32((uint32_t)w16_borland_seed());
    wr32(g_dgroup_3f00_seed);
    wr32(port_init_global_count);
    for (i = 0; i < port_init_global_count; i++) {
        const struct PortInitGlobal *g = &port_init_globals[i];
        unsigned short h[6] = { g->seg, g->off, g->count, g->w16, g->wn, g->kind };
        fwrite(h, 2, 6, cp_f);
        fwrite(g->addr, 1, (size_t)g->count * g->wn, cp_f);
    }
    for (i = 0; i < 4; i++)
        planes[i] = global_at(DGROUP, 0x1c72 + 4 * i);
    {
        uint32_t *bytes = global_at(DGROUP, 0x1c96);
        n = bytes ? *bytes : 0;
    }
    for (i = 0; i < 4; i++) {
        unsigned char *p = planes[i] ? (unsigned char *)*planes[i] : 0;
        uint32_t len = p && n <= 0x40000 ? n : 0;
        wr32(len);
        if (len)
            fwrite(p, 1, len, cp_f);
    }
    fflush(cp_f);
}

/* after each write of GameBrains.step */
void w16_lockstep_step(void *brains)
{
    unsigned char *game, *nation_text, *loaded;
    int16_t *autoplay;
    int turn, step, i;
    if (!w16_lockstep())
        return;
    game = global_at(SEG20, 0x7782);
    nation_text = global_at(SEG20, 0x7810);
    autoplay = global_at(DGROUP, 0x88);
    loaded = global_at(DGROUP, 0x8b);
    if (!game || !nation_text || !autoplay || !loaded)
        return;
    /* the recorder's stores (e2e_turns._on_step_write), once a game is in progress -- not in a
       UI session, where the player stays human */
    if (*(int16_t *)(game + 66) && !noautoplay) {
        for (i = 0; i < 4; i++)
            if (nation_text[0x34 * i + 0x31] == 0)
                nation_text[0x34 * i + 0x31] = 1;
        *autoplay = 1;
        /* and the end of a human turn's: GameBrains_DoHuman clears g_loaded_flag, which no
           one else does -- left set, a loaded game's viceroys before turn_nation never play
           again (tests/lockstep.py, oracle_main) */
        *loaded = 0;
    }
    turn = *(int16_t *)(game + 14);
    step = *(int16_t *)((unsigned char *)brains + 2);
    if (turn == last_turn && step == last_step)
        return;
    last_turn = turn;
    last_step = step;
    fprintf(log_f, "checkpoint %u: turn %d step %d at tick %lu\n", checkpoints, turn, step, ticks);
    fflush(log_f);
    {
        char mark[64];
        snprintf(mark, sizeof mark, "checkpoint %u turn %d step %d", checkpoints, turn, step);
        w16_trace_mark(mark);          /* the call trace's marker (tests/lockstep.py --calls) */
    }
    dump(turn, step);
    checkpoints++;
    if (*(int16_t *)(game + 66) && turns) {
        if (start_turn == -32768)
            start_turn = turn;
        if (turn - start_turn >= (int)turns) {
            fprintf(log_f, "%u turns played: done\n", turns);
            fclose(log_f);
            fclose(cp_f);
            ExitProcess(0);
        }
    }
}
