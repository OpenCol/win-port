# Two merged records whose size the reconstructions contradict

Found by win-port's porter, which maps a file's own struct onto the shared record of the
same name and so has to ask whether they are the same size.

## Rec

`colonies_swap_words` walks an array of records and its own typedef gives their stride:

    typedef struct {
        char pad0[0x90]; ...  char pad2[0xc2 - 0xba]; ...
    } Rec;                                          /* 202 bytes */

`records_swap_words` has a `Rec` of its own at 78 bytes. The shared model's `Rec` is
**1916**. These are three different structures wearing one name, and `Rec` is the name a
reconstruction reaches for when it does not know what to call something -- so a model
that merges by name merges unrelated things.

A port built on the merged record stepped `r[i]` by 1916 bytes through 202-byte records.
It wrote nothing where the shipped code wrote, on 23 of 29 argument sets.

## Settlement

Eleven reconstructions declare `extern Settlement far g_settlements[]` at SEG20:0xcb9a,
and those that give a layout agree it is **18 bytes**:

    typedef struct { unsigned char x, y, owner, flags, size; signed char mission; } Settlement;
    typedef struct { char pad[2]; unsigned char owner; char rest[0x12 - 3]; } Settlement;
    typedef struct { unsigned char x, y; char rest[16]; } Settlement;  /* 18 bytes */

The shared model's `Settlement` is **32**. Its extent cannot have come from the gap to
the next global, because `g_villages` sits at SEG20:0xcb9c -- two bytes later, a column
view of the same array rather than a separate object.

## Why it is worth saying

Neither is visible from inside a single reconstruction. Each file is self-consistent, and
a per-function differential gives the candidate and the shipped function the same memory,
so a struct only has to be right about the offsets that file touches. The stride is what
a merged record gets wrong, and a stride is only wrong where an array is indexed.

win-port has not changed the model. Where a file's own struct is used as an array and its
size contradicts the record of that name, the porter now keeps the FILE's struct: it is
what the code it came from indexes by. That is a workaround, not an answer -- the record
is still wrong for anything else that uses it.

The general shape, which may be worth a check across the model: a record whose name is a
placeholder (`Rec`, `ARec`, `Item`, `Ctl`) is a name collision waiting to happen, and
merging by name rather than by address is what turns it into a wrong layout. Address is
evidence; a name is not.
