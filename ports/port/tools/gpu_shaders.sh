#!/bin/sh
# gpu_shaders.sh: port/render/rd_gpu.hlsl into rd_gpu_shaders.h, DXBC (5.1) and SPIR-V. Run from the top
# after changing the shaders, and commit both. FXC: the Windows SDK's fxc.exe; DXC: DirectXShaderCompiler
# v1.9.2609 (dxc_2026_09_29.zip, sha256 ad31b1fc...) in build/deps.
set -e
FXC=${FXC:-"/c/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64/fxc.exe"}
DXC=${DXC:-build/deps/dxc-v1.9.2609/x/bin/x64/dxc.exe}
OUT=port/render/rd_gpu_shaders.h
TMP=$(mktemp -d)
W=$(cygpath -w "$TMP")
export MSYS_NO_PATHCONV=1
"$FXC" /nologo /T vs_5_1 /E vs_main /O3 /Fo "$W\\vs.dxbc" port/render/rd_gpu.hlsl > /dev/null
"$FXC" /nologo /T ps_5_1 /E ps_main /O3 /Fo "$W\\ps.dxbc" port/render/rd_gpu.hlsl > /dev/null
"$DXC" -nologo -spirv -fspv-target-env=vulkan1.0 -fvk-use-dx-layout -T vs_6_0 -E vs_main -O3 -Fo "$W\\vs.spv" port/render/rd_gpu.hlsl
"$DXC" -nologo -spirv -fspv-target-env=vulkan1.0 -fvk-use-dx-layout -T ps_6_0 -E ps_main -O3 -Fo "$W\\ps.spv" port/render/rd_gpu.hlsl
# with rd_soft.c's exact edges
"$FXC" /nologo /T ps_5_1 /E ps_main /D EXACT_EDGES=1 /O3 /Fo "$W\\psx.dxbc" port/render/rd_gpu.hlsl > /dev/null
"$DXC" -nologo -spirv -fspv-target-env=vulkan1.0 -fvk-use-dx-layout -T ps_6_0 -E ps_main -D EXACT_EDGES=1 -O3 -Fo "$W\\psx.spv" port/render/rd_gpu.hlsl

array() {
    echo "static const unsigned char $1[] = {"
    od -An -v -tu1 "$2" | sed 's/  */ /g; s/^ //; s/ $//; s/ /, /g; s/$/,/; s/^/    /'
    echo "};"
}

{
    echo "/* rd_gpu_shaders.h  rd_gpu.hlsl, built by port/tools/gpu_shaders.sh with fxc and dxc: do not edit */"
    echo
    array rd_gpu_vs_dxbc "$TMP/vs.dxbc"
    echo
    array rd_gpu_ps_dxbc "$TMP/ps.dxbc"
    echo
    array rd_gpu_vs_spirv "$TMP/vs.spv"
    echo
    array rd_gpu_ps_spirv "$TMP/ps.spv"
    echo
    array rd_gpu_ps_exact_dxbc "$TMP/psx.dxbc"
    echo
    array rd_gpu_ps_exact_spirv "$TMP/psx.spv"
} > "$OUT"
rm -rf "$TMP"
echo "$OUT"
