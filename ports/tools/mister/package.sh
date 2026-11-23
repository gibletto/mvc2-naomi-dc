#!/bin/sh
# usage: tools/mister/package.sh VERSION OUTDIR   (Git Bash)
# The MiSTer package: OUTDIR/mvc2-mister-VERSION.tar.gz (files below /media/fat, with games/mvc2/MANIFEST) and
# mvc2_setup.sh beside it, which installs it. PND_EXE: the game (build-pnd/mvc2-pinedra); WRAPPER: MiSTer_MVC2
# (build-hps.sh's, in build/mister).
set -e
v=${1:?usage: package.sh VERSION OUTDIR} out=${2:?usage: package.sh VERSION OUTDIR}
port=$(cd "$(dirname "$0")/../.." && pwd)
exe=${PND_EXE:-$port/build-pnd/mvc2-pinedra} wrapper=${WRAPPER:-$port/build/mister/MiSTer_MVC2}
name=mvc2-mister-$v
s=$out/$name
rm -rf "$s" && mkdir -p "$s/games/mvc2/bin" "$s/games/mvc2/licenses/MiSTer_MVC2/source" "$s/_Other" "$s/Scripts"
cp "$wrapper" "$s/MiSTer_MVC2"
cp "$exe" "$s/games/mvc2/bin/mvc2-pinedra"
cp "$port/port/mister/mvc2-pinedra.sh" "$s/games/mvc2/mvc2-pinedra"
cp "$port/MISTER.md" "$s/games/mvc2/README.md"
lic=$port/LICENSE; [ -f "$lic" ] || lic=$port/../LICENSE    # the public tree has it at its root
cp "$lic" "$s/games/mvc2/licenses/LICENSE.txt"
cp "$port/NOTICE.md" "$s/games/mvc2/licenses/NOTICE.md"
cp "$port/tools/mister/agpl/LICENSE" "$port/tools/mister/agpl/NOTICE.md" "$s/games/mvc2/licenses/MiSTer_MVC2/"
cp "$port/tools/mister/agpl/wrapper/"* "$port/tools/mister/build-hps.sh" "$s/games/mvc2/licenses/MiSTer_MVC2/source/"
cp "$port/tools/mister/mvc2_setup.sh" "$s/Scripts/mvc2_setup.sh"
# one setname for both (one MiSTer.ini section, one button map); the wrapper takes the mode from the file's path
for m in Arcade:arcade Dreamcast:dc; do
    printf '%s\n' '<mistergamedescription>' '  <rbf>pinedra/config/Pinedra Collection</rbf>' \
        '  <setname same_dir="1">MVC2 Pinedra</setname>' "  <file delay=\"0\" type=\"f\" index=\"0\" path=\"${m#*:}\"/>" \
        '</mistergamedescription>' > "$s/_Other/MVC2 ${m%%:*} (Pinedra).mgl"
done
echo "$v" > "$s/games/mvc2/VERSION"
(cd "$s" && find . -type f ! -path ./games/mvc2/MANIFEST | sed 's|^\./||' | LC_ALL=C sort |
    while IFS= read -r f; do sha256sum "$f"; done | sed 's/ \*/  /' > games/mvc2/MANIFEST)
tar --sort=name --owner=0 --group=0 --numeric-owner --mode='u+rwX,go+rX,go-w' -C "$s" -czf "$out/$name.tar.gz" .
cp "$port/tools/mister/mvc2_setup.sh" "$out/mvc2_setup.sh"
(cd "$out" && sha256sum "$name.tar.gz" mvc2_setup.sh > "$name.sha256")
rm -rf "$s"
ls -l "$out/$name.tar.gz" "$out/mvc2_setup.sh"
