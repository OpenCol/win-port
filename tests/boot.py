#!/usr/bin/env python3
"""Does the linked executable LOAD and RUN?

Runs under the byte-exact repo's interpreter, because the Win16 emulator lives there.
This is the third check in this repo and it covers what neither of the others can:

  tests/verify_segments.py   the data is the shipped data          (at rest)
  tests/differential.py      each ported function behaves          (one at a time)
  tests/boot.py              the pieces FIT and the image runs     (all together)

The differential says nothing about calling convention, stack discipline, segment
arithmetic or whether DGROUP overflowed. A loader does.

What it asserts, and nothing more: the file is an NE the loader accepts, the startup
reaches WinMain, WinMain returns, and no instruction faulted. With WinMain still a
placeholder that is the whole of the program's behaviour -- the point is that the LINK is
sound, and this fails the day it stops being.

    tests/boot.py [exe]
"""
import json, os, sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = os.environ.get('COL_SRC_REPO',
                     os.path.expanduser('~/Repositories/dderevjanik/col-win-re'))
sys.path.insert(0, os.path.join(SRC, 'tools', 're_agent'))
sys.path.insert(0, os.path.join(SRC, 'tools'))

EXE = sys.argv[1] if len(sys.argv) > 1 else os.path.join(REPO, 'build', 'link',
                                                         'COLONIZE.EXE')


def main():
    import ne_dump, ne_relocs, win16emu as W
    from gameio import Game
    cache = os.path.join(REPO, 'build', 'link', 'ne')
    os.makedirs(cache, exist_ok=True)
    ne = os.path.join(cache, 'boot.json')
    rel = os.path.join(cache, 'boot.relocs.json')
    json.dump(ne_dump.dump(EXE), open(ne, 'w'), default=list)
    json.dump(ne_relocs.walk(EXE), open(rel, 'w'), default=list)

    image = W.Image(exe=EXE, ne_json=ne, reloc_json=rel)
    # The emulator's DGROUP is a module constant set to 34, the SHIPPED binary's automatic
    # data segment. This executable is a fresh link with seven segments and its DGROUP is
    # segment 7, so the constant is pointed at this image's own header instead. Set here,
    # in this repo, rather than changed over there: that repo is shared and its number is
    # right for the binary it was written for.
    W.DGROUP = image.header['auto_data_seg']
    g = Game(image=image)
    boot = g.start(2_000_000)
    st = g.state()
    apis = [e['api'] for e in g.sim.log]
    # How a Borland Windows program ENDS: the startup calls WinMain, and when WinMain
    # returns the runtime exits through INT 21h AH=4Ch. The emulator's own
    # `program_returned` flag is keyed to the shipped binary's exit path and stays False
    # here, and because its INT 21h handler returns rather than terminating, that call
    # then repeats -- 1,268 times in 2,000,000 instructions. So the exit is read from the
    # API log, and the repeats are the proof that nothing after it matters.
    exited = 'INT 21h AH=4c' in apis
    at = apis.index('INT 21h AH=4c') if exited else None
    print('%s' % os.path.relpath(EXE, REPO))
    print('  instructions : %d' % g.instructions)
    print('  program ended: %s' % st['program_returned'])
    print('  api calls    : %d before exit (%s)'
          % (at if exited else len(apis), ', '.join(apis[:at if exited else 8])))
    print('  unimplemented: %s' % (st['unimplemented'] or 'none'))
    print('  stack drift  : %s' % st['stack_drift'])
    err = boot.get('error') if isinstance(boot, dict) else None
    if err:
        print('  FAULTED      : %s' % err)
        return 1
    if not (exited or st['program_returned']):
        print('  the program never reached its exit: pc %s' % st['pc'])
        return 1
    print('  OK: the loader accepted the image, the startup ran, WinMain returned '
          'and the runtime exited')
    return 0


if __name__ == '__main__':
    sys.exit(main())
