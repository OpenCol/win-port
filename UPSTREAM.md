# UPSTREAM

Requests for col-win-re, which this repository imports from and never edits (roadmap rule 4).
Each entry: what is wrong in the corpus, the evidence (a command and its output, an address,
a lockstep divergence), and what this port does meanwhile.

## 1. Three records whose size the model and the corpus disagree on (W1, 2026-10-07)

`analysis/model.json` (and the `analysis/port-header.h` made from it) gives a size that the
corpus's own definition in `include/records.h` does not have. Measured by compiling both under
MinGW i686 with Borland's widths (`int` 16-bit): of 105 sizes the header asserts, these differ:

| Record | Model | Corpus | Note |
| --- | ---: | ---: | --- |
| `MemBlock` | 2 | 14 | the model seems to size it from its first member |
| `Settlement` | 32 | 18 | the 18-byte record is the settlement the code indexes (`g_settlements`); the community save-file spec agrees |
| `WinGRec` | 8 | 10 | five words in the corpus |

Here: the port takes the corpus's records (they compiled to the shipped bytes), and
`tools/gen_headers.py` leaves those three size checks out (`SIZE_FROM_CORPUS`).

## 2. analysis/port-header.h is older than the model (W1, 2026-10-07)

At 508d4d0d0 the header still names the nation-text record `Nation34` and lacks fields renamed
since (`NationText.control`, `Colony.bonus_hammers`, `Unit.flags2`, ...): 141 files failed on
"has no member" against it. Regenerating it at that commit (`check_header_compiles.py --keep`)
gives a header that compiles fewer files (2,444 against 2,723), because it declares fewer
Windows API functions and globals. Here: `gen_headers.py` takes the corpus's records from
records.h in place of the header's, and maps `Nation34` to `NationText`.
