# 133 callees are declared two ways, and neither harness can see it

Reproduce with `tools/consistency.py --list` in win-port.

## What it is

Every reconstruction declares the callees it uses. Nothing ever compares two files'
declarations of the same callee, because the corpus is one function per file and each is
built on its own. Put them side by side and 133 callees are declared in contradictory
ways:

      66  disagree about what the callee returns
      55  disagree about a parameter type
      12  disagree about how many arguments it takes

and, counted per call site rather than per callee, **65 call sites in 60 files push a
number of arguments the callee's own definition contradicts**.

A sample, with the file each spelling comes from:

    CopyToPort        void (void *, void *, POINT *, POINT *)   port_blit_or_copy.c
                      void (void *, void *, RECT *, RECT *)     MapWin_RevealHiddenTerrain.c

    FillColor         void (GRPort *, col_i16 *, col_i16)       GRPort_FillColor_all.c
                      void (Screen *, RECT *, col_i16)          track_difficulty.c
                      void (Win *, RECT *, col_i16)             cd_draw_time.c

    BringWindowToTop  col_i16 pascal (col_u16)                  main_window_message_handler.c
                      void pascal (col_u16)                     MSSetFocus.c

    MSSetCursor       declared with 2 arguments in MSSetCursor_of.c;
                      its own definition in MSSetCursor.c takes 3

`POINT` is four bytes and `RECT` is eight, so `CopyToPort`'s two readings are not two
names for one thing.

## Why no test catches it

A per-function differential runs the candidate beside the shipped function and gives
both the same callees -- it points every call at the shipped implementation, which is
what isolates the function under test. That is the right experiment, and it is exactly
why this is invisible: when a call pushes one word too few, the shipped callee reads the
same wrong stack on both sides and the two agree. Writes agree, the return agrees, the
stack delta agrees. 50 of the 60 files with a contradictory arity are currently green on
both of win-port's harnesses.

The byte match did cover this, because a call that pushes two words where the shipped
code pushes three does not assemble to the shipped bytes. It is one of the things a
behavioural port gives up, and it does not announce itself.

## What settles each one

Not a head count. Counting spellings has been wrong every time it was checked here --
`g_game_flags` is spelt five ways and the most-repeated one is a byte where the shipped
code does `XOR WORD [0x7784],0x2000`.

      21  are Windows APIs, and windows.h settles them
     108  have a reconstruction of their own, and its DEFINITION settles them:
           the definition states the signature, a call site only reads it
       4  have neither -- ATOL, LTOA1, memset_rtl, sub_1048_d48b -- and for those
           the disagreement is the whole of what is known

The 108 are mechanical: for each, take the definition's arity and return type, and
correct every call site that disagrees. The four are not, and the honest thing there is
to say the signature is unknown rather than pick the commoner spelling.

## Note

col-win-re's own `2ef103193` is "a far pointer where two words go, and 148 more places
the argument count disagrees", so this is already being worked on there. This is a list
from the other side of the same problem, produced by a build that compares declarations
rather than by reading call sites -- it may be useful as a cross-check, and the tool that
produces it is `tools/consistency.py`.

win-port has NOT corrected any of these. Both harnesses pass the affected ports, and
which side of each contradiction is right is a question for the reconstructions, not for
a port of them.
