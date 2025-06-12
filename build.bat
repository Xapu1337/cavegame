@echo off
if not exist build (
    mkdir build
)

pushd build

cl /nologo /std:c11 /EHsc /D_CRT_SECURE_NO_WARNINGS ..\build.c /link d3d11.lib dxguid.lib d3dcompiler.lib user32.lib gdi32.lib kernel32.lib winmm.lib ole32.lib shlwapi.lib shcore.lib dbghelp.lib runtimeobject.lib /OUT:cgame.exe

popd
