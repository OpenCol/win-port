"""Where the corpus contradicts itself about a signature.

One function per file means no build ever puts two ports' declarations of the same
callee side by side. This does, and 133 callees turn out to disagree -- about a return
type, a parameter type, or how many arguments there are. None of it is visible to either
harness: both the shipped function and the port call the SAME shipped callee, so a call
pushing one word too few is read the same wrong way on both sides and the comparison
agrees. That is precisely the hole the byte match used to cover.

WHAT SETTLES EACH ONE. Not a head count -- counting has been wrong every time it was
checked here. A Windows API is settled by windows.h. A game function is settled by its
own reconstruction's DEFINITION, for the same reason a global's width is settled by the
instruction that stores to it: the definition states the signature, a call site only
reads it. Three have neither, and for those the disagreement is all there is.

    tools/consistency.py            # the counts, and what would settle each
    tools/consistency.py --list     # every one, with its spellings
"""
import argparse, collections, glob, os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import link as L                                                      # noqa: E402
import port as P                                                      # noqa: E402


def parts(sig):
    ret, args = sig.split(' (', 1)
    return ret.strip().replace(' pascal', ''), [a.strip()
                                                for a in args.rstrip(')').split(',')]


def compatible(a, b):
    """`void *` is a WEAKER reading of a pointer, not a contradiction of it."""
    if a == b:
        return True
    return a.endswith('*') and b.endswith('*') and ('void *' in (a, b)
                                                    or 'char *' in (a, b))


def conflicts():
    """{name: (what disagrees, {signature: the files that spell it that way})}."""
    seen = collections.defaultdict(lambda: collections.defaultdict(set))
    for x in sorted(glob.glob(os.path.join(REPO, 'src', '*', '*.c'))):
        for n, sig in L.declares(open(x).read()).items():
            seen[n][sig].add(os.path.basename(x))
    out = {}
    for n, spellings in seen.items():
        ss = sorted(spellings)
        for i in range(len(ss)):
            for j in range(i + 1, len(ss)):
                r1, a1 = parts(ss[i])
                r2, a2 = parts(ss[j])
                if len(a1) != len(a2):
                    out[n] = ('how many arguments it takes', spellings)
                elif not compatible(r1, r2):
                    out[n] = ('what it returns', spellings)
                elif any(not compatible(x, y) for x, y in zip(a1, a2)):
                    out[n] = ('a parameter type', spellings)
                if n in out:
                    break
            if n in out:
                break
    return out


def definitions_index():
    """{name: its own reconstruction's argument list}, the thing that settles it."""
    idx = {}
    for p in sorted(glob.glob(os.path.join(P.SRC_REPO, 'matched', '*', '*.c'))):
        nm = os.path.splitext(os.path.basename(p))[0]
        blank = P.blank_comments(open(p).read())
        for name, _body, at, _end in P.definitions(blank):
            if name == nm:
                m = P.DEF.search(blank[at:])
                if m:
                    idx[nm] = m.group('args')
                break
    return idx


def arity_against_definition(idx):
    """[(callee, the file, what it says, what the definition takes)]."""
    out = []
    for x in sorted(glob.glob(os.path.join(REPO, 'src', '*', '*.c'))):
        for n, sig in L.declares(open(x).read()).items():
            if n not in idx:
                continue
            want = idx[n]
            wn = 0 if want.strip() in ('', 'void') else len(want.split(','))
            args = sig.split(' (', 1)[1].rstrip(')')
            gn = 0 if args.strip() in ('', 'void') else len(args.split(','))
            if wn != gn:
                out.append((n, os.path.basename(x), gn, wn))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--list', action='store_true')
    a = ap.parse_args()
    bad = conflicts()
    kinds = collections.Counter(w for w, _ in bad.values())
    print('%d callees are declared in contradictory ways across the ports' % len(bad))
    for k, n in kinds.most_common():
        print('  %4d  disagree about %s' % (n, k))

    idx = definitions_index()
    api = [n for n in bad if n.upper() in P.WINAPI]
    own = [n for n in bad if n not in api and n in idx]
    none = [n for n in bad if n not in api and n not in own]
    print('\nwhat settles them:')
    print('  %4d  a Windows API: windows.h says' % len(api))
    print('  %4d  a reconstruction of their own: its definition says' % len(own))
    print('  %4d  neither, so the disagreement is all there is: %s'
          % (len(none), ', '.join(sorted(none))))

    ar = arity_against_definition(idx)
    print('\n%d call sites in %d files push a number of arguments their callee\'s own'
          % (len(ar), len({f for _n, f, _g, _w in ar}))
          + '\ndefinition contradicts. NEITHER harness can see this: both sides call the'
          + '\nsame shipped callee, so one word too few is read the same wrong way twice.')
    if a.list:
        for n, f, g, w in ar:
            print('   %-30s %-34s says %d, its definition takes %d' % (n, f, g, w))
        print()
        for n, (what, spellings) in sorted(bad.items()):
            print('%s -- %s' % (n, what))
            for sig, who in sorted(spellings.items()):
                print('     %-46s %s' % (sig, ', '.join(sorted(who)[:3])))
    return 0


if __name__ == '__main__':
    sys.exit(main())
