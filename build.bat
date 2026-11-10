@echo off
setlocal
pushd "%~dp0"
rem Marvel vs. Capcom 2 (NAOMI) - the program ROM, epr-23085a.ic11 (same steps as the Makefile)
rem Set SHC to the bin folder of Hitachi SHC 5.1 Release 08 and SDK_LIBS to the SDK's library
rem folders (see README.md, Building).
if "%SHC%"=="" echo set SHC to the folder of SHC 5.1 Release 08& goto fail
if "%SDK_LIBS%"=="" echo set SDK_LIBS to the SDK library folders, separated by ;& goto fail
set SHC_LIB=%SHC%
set SHC_TMP=build\tmp
rem Capcom's options, from the response file left in the ROM padding (README.md)
set COPTS=-cpu=sh4 -endian=little -fpu=single -round=nearest -macsave=0 -pic=0 -optimize=1 -size -string=const -extra=a=400 -define=_STRICT_UNION_,__DEV_TYPE_DC__
set AOPTS=-cpu=sh4 -endian=little
for %%d in (build\obj\test build\tmp build\data) do if not exist %%d mkdir %%d

rem what the source does not have, cut from your own ROM in rom\
python tools\mvc2rom.py rom build\orig || goto fail

echo compiling
for %%f in (src\*.c) do (
	"%SHC%\shc.exe" %%f %COPTS% -include=include -object=build\obj\%%~nf.obj >build\obj\shc.log 2>&1 || (type build\obj\shc.log & goto fail)
	findstr /c:") : " build\obj\shc.log
)
for %%f in (src\*.src) do (
	"%SHC%\asmsh.exe" %%f -object=build\obj\%%~nf.obj %AOPTS% >build\obj\asm.log || (type build\obj\asm.log & goto fail)
)
rem Sega's modules: each library's options are in src\lib\LIBRARY\options (a module's own in
rem MODULE.options)
for /d %%d in (src\lib\*) do (
	for %%f in (%%d\*.c) do call :libc %%d %%f || goto fail
	for %%f in (%%d\*.src) do (
		"%SHC%\asmsh.exe" %%f -object=build\obj\%%~nf.obj %AOPTS% >build\obj\asm.log || (type build\obj\asm.log & goto fail)
	)
)
rem the test program's own modules; the rest of it is main's start-up and work, and Sega's
for %%f in (src\test\*.c) do (
	"%SHC%\shc.exe" %%f %COPTS% -include=src\test,include -object=build\obj\test\%%~nf.obj >build\obj\shc.log 2>&1 || (type build\obj\shc.log & goto fail)
	findstr /c:") : " build\obj\shc.log
)
for %%f in (src\test\*.src) do (
	"%SHC%\asmsh.exe" %%f -object=build\obj\test\%%~nf.obj %AOPTS% >build\obj\asm.log || (type build\obj\asm.log & goto fail)
)
del build\obj\shc.log build\obj\asm.log

echo copying the libraries
rem build\sdk\N has those the links take from the Nth folder of SDK_LIBS
set N=0
for %%d in ("%SDK_LIBS:;=" "%") do call :sdk "%%~d" || goto fail

echo linking
"%SHC%\lnk.exe" -subcommand=mvc2.sub >build\lnk.log || (findstr /b /c:"**" build\lnk.log & goto fail)
"%SHC%\lnk.exe" -subcommand=mvc2test.sub >build\lnktest.log || (findstr /b /c:"**" build\lnktest.log & goto fail)
for %%f in (mvc2 test1 test2 test3 test4 mvc2test fill tail) do if exist build\%%f.bin del build\%%f.bin
pushd build
"%SHC%\elf2bin.exe" -b ff -s 0C020000 mvc2.abs mvc2.bin >fill.log 2>&1
rem the test program's image is cut in four, with a hole after each of the first three:
rem left-overs of older builds that no build gives, from your ROM
"%SHC%\elf2bin.exe" -b ff -s 0C020000 -e 0C046C00 mvc2test.abs test1.bin >>fill.log 2>&1
"%SHC%\elf2bin.exe" -b ff -s 0C048C00 -e 0C049400 mvc2test.abs test2.bin >>fill.log 2>&1
"%SHC%\elf2bin.exe" -b ff -s 0C04B000 -e 0C055C00 mvc2test.abs test3.bin >>fill.log 2>&1
"%SHC%\elf2bin.exe" -b ff -s 0C056000 mvc2test.abs test4.bin >>fill.log 2>&1
popd
copy /b build\test1.bin+build\orig\hole_3E6C00.bin+build\test2.bin+build\orig\hole_3E9400.bin+build\test3.bin+build\orig\hole_3F5C00.bin+build\test4.bin build\mvc2test.bin >nul

rem the 0xFF after the main program (up to ROM 0x250000) and after the test program (to the end)
rem are cut to length from a link of nothing else
>build\fill.src echo           .SECTION  FILL,DATA,ALIGN=4
echo           .DATAB.B  H'10000,H'FF>>build\fill.src
echo           .END>>build\fill.src
echo input build\fill.obj>build\fill.sub
echo output build\fill.abs>>build\fill.sub
echo elf>>build\fill.sub
echo form a>>build\fill.sub
echo start FILL(0)>>build\fill.sub
echo exit>>build\fill.sub
"%SHC%\asmsh.exe" build\fill.src -object=build\fill.obj %AOPTS% >>build\fill.log || goto fail
"%SHC%\lnk.exe" -subcommand=build\fill.sub >>build\fill.log || goto fail
for %%f in (build\mvc2.bin) do set /a PAD=0x250000-%%~zf
rem the length in hexadecimal
%ComSpec% /c exit %PAD%
"%SHC%\elf2bin.exe" -s 0 -e %=ExitCode% build\fill.abs build\fill.bin >>build\fill.log 2>&1
for %%f in (build\mvc2test.bin) do set /a PAD=0x40000-%%~zf
%ComSpec% /c exit %PAD%
"%SHC%\elf2bin.exe" -s 0 -e %=ExitCode% build\fill.abs build\tail.bin >>build\fill.log 2>&1

rem the characters' data (data\*.c): each file is a part of a character's pack, linked alone at 0
rem (its tables hold offsets from its own start); tools\chrpack.py packs them as the ROM has them
echo compiling the characters' data
for %%f in (data\*.c) do call :data %%~nf || goto fail
python tools\chrpack.py build\data build\chardata.bin || goto fail

rem the main program, the characters' data, seven model files that the game never reads (from
rem your ROM), and the test program at 0x3C0000
copy /b build\mvc2.bin+build\fill.bin+build\chardata.bin+build\orig\data_340000.bin+build\mvc2test.bin+build\tail.bin build\epr-23085a.ic11 >nul
certutil -hashfile build\epr-23085a.ic11 SHA1 | findstr /i /c:"f58c31b245fc33fa541f9f074548402a63f7c3d3" >nul || (echo build\epr-23085a.ic11 is not the arcade's & goto fail)
echo build\epr-23085a.ic11 is the arcade's: SHA-1 f58c31b245fc33fa541f9f074548402a63f7c3d3
popd
exit /b 0

:data
rem compile data\%1.c, link it at 0 and convert it to build\data\%1.bin
"%SHC%\shc.exe" data\%1.c %COPTS% -include=include -object=build\data\%1.obj >build\data\shc.log 2>&1 || (type build\data\shc.log & exit /b 1)
findstr /c:") : " build\data\shc.log
echo input build\data\%1.obj>build\data\link.sub
echo output build\data\%1.abs>>build\data\link.sub
echo elf>>build\data\link.sub
echo form a>>build\data\link.sub
echo start C(0)>>build\data\link.sub
echo exit>>build\data\link.sub
"%SHC%\lnk.exe" -subcommand=build\data\link.sub >build\data\lnk.log || (type build\data\lnk.log & exit /b 1)
if exist build\data\%1.bin del build\data\%1.bin
"%SHC%\elf2bin.exe" build\data\%1.abs build\data\%1.bin >build\data\lnk.log 2>&1 || exit /b 1
exit /b 0

:sdk
rem copy the libraries that the links take from %1, the next folder of SDK_LIBS
set /a N+=1
if not exist build\sdk\%N% mkdir build\sdk\%N%
for /f "tokens=4,5 delims=\( " %%a in ('findstr /c:"input build\sdk" mvc2.sub mvc2test.sub') do (
	if "%%a"=="%N%" if not exist build\sdk\%N%\%%b copy /y "%~1\%%b" build\sdk\%N% >nul || exit /b 1
)
exit /b 0

:libc
rem compile %2, a module of the library in %1; one with inline assembler is compiled to assembler
rem source and assembled, as Sega's makefiles did
set OPTS=%1\options
if exist %~dpn2.options set OPTS=%~dpn2.options
set /p OPTS=<%OPTS%
findstr /c:"#pragma inline_asm" %2 >nul && goto libasm
"%SHC%\shc.exe" %2 %OPTS% -include=%1,include,build\orig -object=build\obj\%~n2.obj >build\obj\shc.log 2>&1 || (type build\obj\shc.log & exit /b 1)
findstr /c:") : " build\obj\shc.log
exit /b 0
:libasm
"%SHC%\shc.exe" %2 %OPTS% -code=asmcode -include=%1,include,build\orig -object=build\tmp\%~n2.src >build\obj\shc.log 2>&1 || (type build\obj\shc.log & exit /b 1)
"%SHC%\asmsh.exe" build\tmp\%~n2.src -object=build\obj\%~n2.obj %AOPTS% >build\obj\asm.log || (type build\obj\asm.log & exit /b 1)
exit /b 0

:fail
popd
exit /b 1
