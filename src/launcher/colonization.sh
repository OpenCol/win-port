#!/bin/sh
# The release kit's launcher for Linux and macOS (roadmap W10): ./colonization [GAME_FOLDER]
#
# Runs the port (bin/) under Wine in the folder of your copy of Colonization for Windows: the
# port reads the game's own files there and saves there, as the original did. No game file is
# in the kit. The folder is: the argument; this folder, if the kit was unpacked into the game;
# the one used last time (~/.config/colonization-port/gamedir); $COLONIZATION_DIR; Steam's
# Colonization folder.
#
# A folder with no COLONIZE.EXE but Steam's COLONIZE.ISO (the DOS release's disc image, which
# carries the Windows edition too), in it or one folder down, is unpacked once into a game folder
# of the kit's own -- ~/Library/Application Support/colonization-port/game on macOS,
# ~/.local/share/colonization-port/game on Linux -- by bin/colonization-import.exe (the Windows
# launcher's --import, src/launcher/isoimport.c) under Wine, and that folder is used.
# COLONIZE.EXE must be the 1995 release, patched or the original (its SHA-256 is checked); the
# original lacks the patch's COLDATA9.DLL and COCKGUN.WAV (the Jukebox's Control Panel, the
# Cheat menu's dialogs).
#
# Wine: $PORT_WINE, else `wine` on PATH, else Gcenx's Wine app in (~)/Applications, else wine64.
# $PORT_DESKTOP sets the game's desktop window size. On macOS use a current Wine (e.g. Gcenx's
# wine-devel 11 from github.com/Gcenx/macOS_Wine_builds): Apple's Game Porting Toolkit Wine 7.7
# stops the game the first time it plays a sound.
set -e
PATCHED=59a0e173452924603112cf410483021c02ef0cd3512e310d8966b21e0c176291
PREPATCH=ae7d9149f056766a534fe8f0006c9512aac1e1004e9776ab3cca818e4833b650
here=$(cd "$(dirname "$0")" && pwd)
conf="${XDG_CONFIG_HOME:-$HOME/.config}/colonization-port"
if [ "$(uname)" = Darwin ]; then
    unpacked="$HOME/Library/Application Support/colonization-port/game"
    steam="$HOME/Library/Application Support/Steam/steamapps/common"
else
    unpacked="${XDG_DATA_HOME:-$HOME/.local/share}/colonization-port/game"
    steam="$HOME/.local/share/Steam/steamapps/common"
    [ -d "$steam" ] || steam="$HOME/.steam/steam/steamapps/common"
fi

winpath() { printf 'Z:%s' "$(printf %s "$1" | tr / '\\')"; }   # Wine's name for a Unix path (Z: is /)

# Wine: $PORT_WINE; else `wine` on PATH; else a Gcenx Wine app in /Applications or ~/Applications
# (Homebrew's wine@devel / wine-stable casks put them there); else wine64 -- on a Mac often Apple's
# Game Porting Toolkit, the last resort
wine=""
for w in "$PORT_WINE" wine \
         "/Applications/Wine Devel.app/Contents/Resources/wine/bin/wine" \
         "/Applications/Wine Stable.app/Contents/Resources/wine/bin/wine" \
         "/Applications/Wine Staging.app/Contents/Resources/wine/bin/wine" \
         "$HOME/Applications/Wine Devel.app/Contents/Resources/wine/bin/wine" \
         "$HOME/Applications/Wine Stable.app/Contents/Resources/wine/bin/wine" wine64; do
    if [ -n "$w" ] && command -v "$w" >/dev/null 2>&1; then wine=$w; break; fi
done
if [ -z "$wine" ]; then
    echo "Colonization: Wine is needed (https://www.winehq.org; on macOS Gcenx's wine-devel builds)." >&2
    exit 5
fi
case "$("$wine" --version 2>/dev/null)" in
    *"Game Porting Toolkit"*|wine-7.7*)
        echo "Colonization: warning: $("$wine" --version) stops the game when a sound plays; use a" >&2
        echo "current Wine (PORT_WINE=/path/to/wine to choose one)." >&2 ;;
esac

find_exe() {    # the game's COLONIZE.EXE in $1, whatever its case
    for n in COLONIZE.EXE colonize.exe Colonize.exe; do
        [ -f "$1/$n" ] && { echo "$1/$n"; return 0; }
    done
    return 1
}

find_iso() {    # Steam's COLONIZE.ISO in $1 or one folder down
    for f in "$1"/COLONIZE.ISO "$1"/colonize.iso "$1"/*/COLONIZE.ISO "$1"/*/colonize.iso; do
        [ -f "$f" ] && { echo "$f"; return 0; }
    done
    return 1
}

game=""
for d in "$1" "$here" "$(cat "$conf/gamedir" 2>/dev/null)" "$COLONIZATION_DIR" \
         "$steam/Sid Meier's Colonization" "$steam/Colonization"; do
    [ -n "$d" ] && [ -d "$d" ] || continue
    if find_exe "$d" >/dev/null; then game=$(cd "$d" && pwd); break; fi
    if iso=$(find_iso "$d"); then
        if find_exe "$unpacked" >/dev/null; then
            echo "Colonization: using $unpacked, unpacked from $iso before" >&2
        else
            echo "Colonization: no COLONIZE.EXE in $d, but $iso: unpacking it into $unpacked" >&2
            mkdir -p "$unpacked"
            "$wine" "$here/bin/colonization-import.exe" --import "$(winpath "$iso")" "$(winpath "$unpacked")" ||
                { echo "Colonization: $iso could not be unpacked (see above)." >&2; exit 6; }
        fi
        game=$unpacked
        break
    fi
done
if [ -z "$game" ]; then
    echo "Colonization: where is your copy? Run: $0 /path/to/the/folder/with/COLONIZE.EXE" >&2
    echo "(or Steam's Colonization folder, the one with COLONIZE.ISO)" >&2
    exit 1
fi
exe=$(find_exe "$game")
if command -v sha256sum >/dev/null 2>&1; then sum=$(sha256sum "$exe" | cut -d' ' -f1)
else sum=$(shasum -a 256 "$exe" | cut -d' ' -f1); fi
if [ "$sum" = "$PREPATCH" ]; then
    echo "Colonization: $exe is the original 1995 release: without the patch's COLDATA9.DLL and" >&2
    echo "COCKGUN.WAV, the Jukebox's Control Panel and the Cheat menu's dialogs are missing." >&2
elif [ "$sum" != "$PATCHED" ]; then
    echo "Colonization: $exe is not a build this port knows (SHA-256 $sum)." >&2
    echo "It needs the 1995 release of Colonization for Windows (SHA-256 $PATCHED, or the original's)." >&2
    exit 4
fi
mkdir -p "$conf" && echo "$game" > "$conf/gamedir"

prog=$(ls "$here"/bin/colonize.exe.so "$here"/bin/colonize.exe 2>/dev/null | head -1)
# the port's log (and a crash's report and minidump) beside this script, as Wine names it
PORT_LOG=$(winpath "$here/colonization-port.log")
export PORT_LOG
echo "Colonization: the log is $here/colonization-port.log" >&2
cd "$game"
# through Wine's explorer, in a desktop window of its own ($PORT_DESKTOP, default 1024x768): a
# Winelib program given to `wine` directly goes to start.exe, which cannot open a .so
exec "$wine" explorer "/desktop=Colonization,${PORT_DESKTOP:-1024x768}" "$prog"
