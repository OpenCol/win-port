#!/bin/sh
# The release kit's launcher for Linux and macOS (roadmap W10): ./colonization [GAME_FOLDER]
#
# Runs the port (bin/) under Wine in the folder of your copy of Colonization for Windows: the
# port reads the game's own files there and saves there, as the original did. No game file is
# in the kit. The folder is: the argument; this folder, if the kit was unpacked into the game;
# the one used last time (~/.config/colonization-port/gamedir); $COLONIZATION_DIR.
# COLONIZE.EXE must be the updated 1995 release (its SHA-256 is checked).
#
# Wine: $PORT_WINE, else `wine` on PATH. $PORT_DESKTOP sets the game's desktop window size. On macOS use a current Wine (e.g. Gcenx's wine-devel
# 11 from github.com/Gcenx/macOS_Wine_builds): Apple's Game Porting Toolkit Wine 7.7 stops the
# game the first time it plays a sound.
set -e
PATCHED=59a0e173452924603112cf410483021c02ef0cd3512e310d8966b21e0c176291
PREPATCH=ae7d9149f056766a534fe8f0006c9512aac1e1004e9776ab3cca818e4833b650
here=$(cd "$(dirname "$0")" && pwd)
conf="${XDG_CONFIG_HOME:-$HOME/.config}/colonization-port"

find_exe() {    # the game's COLONIZE.EXE in $1, whatever its case
    for n in COLONIZE.EXE colonize.exe Colonize.exe; do
        [ -f "$1/$n" ] && { echo "$1/$n"; return 0; }
    done
    return 1
}

game=""
for d in "$1" "$here" "$(cat "$conf/gamedir" 2>/dev/null)" "$COLONIZATION_DIR"; do
    if [ -n "$d" ] && find_exe "$d" >/dev/null; then game=$(cd "$d" && pwd); break; fi
done
if [ -z "$game" ]; then
    echo "Colonization: where is your copy? Run: $0 /path/to/the/folder/with/COLONIZE.EXE" >&2
    exit 1
fi
exe=$(find_exe "$game")
if command -v sha256sum >/dev/null 2>&1; then sum=$(sha256sum "$exe" | cut -d' ' -f1)
else sum=$(shasum -a 256 "$exe" | cut -d' ' -f1); fi
if [ "$sum" = "$PREPATCH" ]; then
    echo "Colonization: $exe is the original 1995 release. The port is built from the updated 1995" >&2
    echo "release (the official patch). Apply that patch to your copy, then start again." >&2
    exit 3
fi
if [ "$sum" != "$PATCHED" ]; then
    echo "Colonization: $exe is not a build this port knows (SHA-256 $sum)." >&2
    echo "It needs the updated 1995 release of Colonization for Windows (SHA-256 $PATCHED)." >&2
    exit 4
fi
mkdir -p "$conf" && echo "$game" > "$conf/gamedir"

wine="${PORT_WINE:-wine}"
if ! command -v "$wine" >/dev/null 2>&1; then
    command -v wine64 >/dev/null 2>&1 && wine=wine64 || {
        echo "Colonization: Wine is needed (https://www.winehq.org; on macOS Gcenx's wine-devel builds)." >&2
        exit 5; }
fi
case "$("$wine" --version 2>/dev/null)" in
    *"Game Porting Toolkit"*|wine-7.7*)
        echo "Colonization: warning: $("$wine" --version) stops the game when a sound plays; use a" >&2
        echo "current Wine (PORT_WINE=/path/to/wine to choose one)." >&2 ;;
esac
prog=$(ls "$here"/bin/colonize.exe.so "$here"/bin/colonize.exe 2>/dev/null | head -1)
# the port's log (and a crash's report and minidump) beside this script, as Wine names it (Z: is /)
PORT_LOG="Z:$(printf %s "$here/colonization-port.log" | tr / '\\')"
export PORT_LOG
echo "Colonization: the log is $here/colonization-port.log" >&2
cd "$game"
# through Wine's explorer, in a desktop window of its own ($PORT_DESKTOP, default 1024x768): a
# Winelib program given to `wine` directly goes to start.exe, which cannot open a .so
exec "$wine" explorer "/desktop=Colonization,${PORT_DESKTOP:-1024x768}" "$prog"
