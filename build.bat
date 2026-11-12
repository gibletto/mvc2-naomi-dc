@echo off
setlocal
pushd "%~dp0"
rem Marvel vs. Capcom 2 (NAOMI) - the program ROM, epr-23085a.ic11 (same steps as the Makefile);
rem build.bat us builds the US set's, epr-23062a.ic22, and build.bat dc the Dreamcast program
rem Set SHC to the bin folder of Hitachi SHC 5.1 Release 08 and SDK_LIBS to the SDK's library
rem folders (see README.md, Building).
if "%SHC%"=="" echo set SHC to the folder of SHC 5.1 Release 08& goto fail
if "%SDK_LIBS%"=="" echo set SDK_LIBS to the SDK library folders, separated by ;& goto fail
set SHC_LIB=%SHC%
set SHC_TMP=build\tmp
rem Capcom's options, from the response file left in the ROM padding (README.md), the program's own
rem date, which the source asks for where a later build differs, and its ROM board (1, the M1)
set OPTS0=-cpu=sh4 -endian=little -fpu=single -round=nearest -macsave=0 -pic=0 -optimize=1 -size -string=const -extra=a=400 -define=_STRICT_UNION_,__DEV_TYPE_DC__
set COPTS=%OPTS0%,BUILD_DATE=20000225,ROM_BOARD=1
set AOPTS=-cpu=sh4 -endian=little -define=ROM_BOARD=1
if /i "%~1"=="dc" goto dc
rem what each build is: the main program's link, the test program's, the objects' folders, the
rem pieces cut from your ROM, the ROM and its SHA-1, and where the test program's image is cut
rem (each start and end), with a hole of the ROM after each of the first three pieces
set MAIN=mvc2
set TEST=mvc2test
set OBJ=build\obj
set TOBJ=build\obj\test
set ORIG=build\orig
set ROMARG=
set ROMFILE=epr-23085a.ic11
set WHOSE=the arcade's
set SETNAME=
set SHA1=f58c31b245fc33fa541f9f074548402a63f7c3d3
set CUTS=0C020000:0C046C00 0C048C00:0C049400 0C04B000:0C055C00 0C056000:
set HOLES=hole_3E6C00 hole_3E9400 hole_3F5C00
if /i not "%~1"=="us" goto build
rem The US set's (build.bat us), for the M2 board: every object again, with the set's own date and
rem ROM_BOARD=2. At 0x340000 it has six of the scenes' model files, which the game reads, and the
rem test program's holes are at other places; both are cut from your US ROM.
set COPTS=%OPTS0%,BUILD_DATE=20000217,ROM_BOARD=2
set AOPTS=-cpu=sh4 -endian=little -define=ROM_BOARD=2
set MAIN=mvc2u
set TEST=mvc2utest
set OBJ=build\obj\us
set TOBJ=build\obj\ustest
set ORIG=build\orig\us
set ROMARG=--us
set ROMFILE=epr-23062a.ic22
set WHOSE=the US arcade's
set SETNAME= (mvsc2u)
set SHA1=877ba02c92082567280afcb1ae40b3bbfc8a63e8
set CUTS=0C020000:0C046C00 0C048800:0C049000 0C04AC00:0C055400 0C055C00:
set HOLES=hole_3E6C00 hole_3E9000 hole_3F5400

:build
set SUBS=%MAIN%.sub %TEST%.sub
for %%d in (%OBJ% %TOBJ% build\tmp build\data) do if not exist %%d mkdir %%d

rem what the source does not have, cut from your own ROM in rom\
python tools\mvc2rom.py %ROMARG% rom %ORIG% || goto fail

echo compiling
for %%f in (src\*.c) do (
	"%SHC%\shc.exe" %%f %COPTS% -include=include -object=%OBJ%\%%~nf.obj >%OBJ%\shc.log 2>&1 || (type %OBJ%\shc.log & goto fail)
	findstr /c:") : " %OBJ%\shc.log
)
for %%f in (src\*.src) do (
	"%SHC%\asmsh.exe" %%f -object=%OBJ%\%%~nf.obj %AOPTS% >%OBJ%\asm.log || (type %OBJ%\asm.log & goto fail)
)
rem Sega's modules: each library's options are in src\lib\LIBRARY\options (a module's own in
rem MODULE.options)
for /d %%d in (src\lib\*) do (
	for %%f in (%%d\*.c) do call :libc %%d %%f || goto fail
	for %%f in (%%d\*.src) do (
		"%SHC%\asmsh.exe" %%f -object=%OBJ%\%%~nf.obj %AOPTS% >%OBJ%\asm.log || (type %OBJ%\asm.log & goto fail)
	)
)
rem the test program's own modules; the rest of it is main's start-up and work, and Sega's
for %%f in (src\test\*.c) do (
	"%SHC%\shc.exe" %%f %COPTS% -include=src\test,include -object=%TOBJ%\%%~nf.obj >%OBJ%\shc.log 2>&1 || (type %OBJ%\shc.log & goto fail)
	findstr /c:") : " %OBJ%\shc.log
)
for %%f in (src\test\*.src) do (
	"%SHC%\asmsh.exe" %%f -object=%TOBJ%\%%~nf.obj %AOPTS% >%OBJ%\asm.log || (type %OBJ%\asm.log & goto fail)
)
del %OBJ%\shc.log %OBJ%\asm.log

echo copying the libraries
rem build\sdk\N has those the links take from the Nth folder of SDK_LIBS
set N=0
for %%d in ("%SDK_LIBS:;=" "%") do call :sdk "%%~d" || goto fail

echo linking
"%SHC%\lnk.exe" -subcommand=%MAIN%.sub >build\lnk%MAIN%.log || (findstr /b /c:"**" build\lnk%MAIN%.log & goto fail)
"%SHC%\lnk.exe" -subcommand=%TEST%.sub >build\lnk%TEST%.log || (findstr /b /c:"**" build\lnk%TEST%.log & goto fail)
for %%f in (%MAIN% test1 test2 test3 test4 %TEST% fill tail) do if exist build\%%f.bin del build\%%f.bin
pushd build
"%SHC%\elf2bin.exe" -b ff -s 0C020000 %MAIN%.abs %MAIN%.bin >fill.log 2>&1
rem the test program's image is cut in four, with a hole after each of the first three:
rem left-overs of older builds that no build gives, from your ROM
set I=0
for %%c in (%CUTS%) do call :cut %%c
popd
set CAT=build\test1.bin
set I=1
for %%h in (%HOLES%) do call :hole %%h
copy /b %CAT% build\%TEST%.bin >nul

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
for %%f in (build\%MAIN%.bin) do set /a PAD=0x250000-%%~zf
rem the length in hexadecimal
%ComSpec% /c exit %PAD%
"%SHC%\elf2bin.exe" -s 0 -e %=ExitCode% build\fill.abs build\fill.bin >>build\fill.log 2>&1
for %%f in (build\%TEST%.bin) do set /a PAD=0x40000-%%~zf
%ComSpec% /c exit %PAD%
"%SHC%\elf2bin.exe" -s 0 -e %=ExitCode% build\fill.abs build\tail.bin >>build\fill.log 2>&1

rem the characters' data (data\*.c): each file is a part of a character's pack, linked alone at 0
rem (its tables hold offsets from its own start); tools\chrpack.py packs them as the ROM has them
echo compiling the characters' data
for %%f in (data\*.c) do call :data %%~nf || goto fail
python tools\chrpack.py build\data build\chardata.bin || goto fail

rem the main program, the characters' data, the model files at 0x340000 (seven that the game never
rem reads, or the US set's six; from your ROM), and the test program at 0x3C0000
copy /b build\%MAIN%.bin+build\fill.bin+build\chardata.bin+%ORIG%\data_340000.bin+build\%TEST%.bin+build\tail.bin build\%ROMFILE% >nul
certutil -hashfile build\%ROMFILE% SHA1 | findstr /i /c:"%SHA1%" >nul || (echo build\%ROMFILE% is not %WHOSE% & goto fail)
echo build\%ROMFILE% is %WHOSE%%SETNAME%: SHA-1 %SHA1%
popd
exit /b 0

:cut
rem %1 is START:END (END empty for the rest) of the next piece of the test program's image
set /a I+=1
for /f "tokens=1,2 delims=:" %%a in ("%1") do (set FROM=%%a& set TO=%%b)
if "%TO%"=="" ("%SHC%\elf2bin.exe" -b ff -s %FROM% %TEST%.abs test%I%.bin >>fill.log 2>&1) else ("%SHC%\elf2bin.exe" -b ff -s %FROM% -e %TO% %TEST%.abs test%I%.bin >>fill.log 2>&1)
set TO=
exit /b 0

:hole
rem the next hole of your ROM, then the piece after it
set /a I+=1
set CAT=%CAT%+%ORIG%\%1.bin+build\test%I%.bin
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
for /f "tokens=4,5 delims=\( " %%a in ('findstr /c:"input build\sdk" %SUBS%') do (
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
"%SHC%\shc.exe" %2 %OPTS% -include=%1,include,%ORIG% -object=%OBJ%\%~n2.obj >%OBJ%\shc.log 2>&1 || (type %OBJ%\shc.log & exit /b 1)
findstr /c:") : " %OBJ%\shc.log
exit /b 0
:libasm
"%SHC%\shc.exe" %2 %OPTS% -code=asmcode -include=%1,include,%ORIG% -object=build\tmp\%~n2.src >%OBJ%\shc.log 2>&1 || (type %OBJ%\shc.log & exit /b 1)
"%SHC%\asmsh.exe" build\tmp\%~n2.src -object=%OBJ%\%~n2.obj %AOPTS% >%OBJ%\asm.log || (type %OBJ%\asm.log & exit /b 1)
exit /b 0

:dc
rem The Dreamcast program, 1ST_READ.BIN of the USA disc, and the characters' files (build.bat dc).
rem SDK_LIBS has two more folders (README.md). Each object that mvc2dc.sub links is compiled from
rem the first of src\dc, src, src\dc\lib and src\lib that has it, with the switches TARGET_DC, the
rem disc's BUILD_DATE and ROM_BOARD=0.
set DCOPTS=%OPTS0%,TARGET_DC,BUILD_DATE=20000529,ROM_BOARD=0
set SUBS=mvc2dc.sub
for %%d in (build\obj\dc build\tmp build\ovl) do if not exist %%d mkdir %%d
echo compiling
for /f "tokens=2" %%i in ('findstr /b /c:"input build\obj\dc" mvc2dc.sub') do call :dcobj %%~ni || goto fail
del build\obj\dc\shc.log build\obj\dc\asm.log 2>nul
echo copying the libraries
set N=0
for %%d in ("%SDK_LIBS:;=" "%") do call :sdk "%%~d" || goto fail
echo linking
"%SHC%\lnk.exe" -subcommand=mvc2dc.sub >build\lnkdc.log || (findstr /b /c:"**" build\lnkdc.log & goto fail)
if exist build\mvc2dc.bin del build\mvc2dc.bin
rem the Dreamcast fills the gaps with 0
pushd build
"%SHC%\elf2bin.exe" -b 00 -s 8C010000 mvc2dc.abs mvc2dc.bin >mvc2dc.log 2>&1
popd
certutil -hashfile build\mvc2dc.bin SHA1 | findstr /i /c:"b0c3604e3494b12f751793a74fc9b494ed67a30c" >nul || (echo build\mvc2dc.bin is not the disc's 1ST_READ.BIN & goto fail)
echo build\mvc2dc.bin is the disc's 1ST_READ.BIN: SHA-1 b0c3604e3494b12f751793a74fc9b494ed67a30c

rem The characters' files, S_PLxx[A-F].BIN: a character's module, src\s_plXX.c compiled for the
rem Dreamcast, is linked six times, at 0CE30000 + 0x8000 for each slot (A to F), beside the main
rem program's names. The linker's symbol file of the main program (build\mvc2dc.FSY) is assembler
rem source, and asmsh assembles it as it is. dcovl.sha1 has the disc's SHA-1s.
echo the characters' files
"%SHC%\asmsh.exe" build\mvc2dc.FSY -object=build\ovl\dcsyms.obj %AOPTS% >build\ovl\asm.log || (type build\ovl\asm.log & goto fail)
rem (s_pl??.c also finds s_play.c, which is not a character's)
for %%f in (src\s_pl??.c) do if /i not "%%~nf"=="s_play" call :dcovl %%~nf || goto fail
del build\ovl\shc.log build\ovl\asm.log build\ovl\lnk.log 2>nul
set BAD=0
for /f "tokens=1,2 delims=* " %%a in (dcovl.sha1) do call :dcsha %%a %%b
if not "%BAD%"=="0" echo %BAD% of the characters' files are not the disc's& goto fail
echo the characters' files are the disc's
popd
exit /b 0

:dcovl
"%SHC%\shc.exe" src\%1.c %DCOPTS% -include=include -object=build\ovl\%1.obj >build\ovl\shc.log 2>&1 || (type build\ovl\shc.log & exit /b 1)
findstr /c:") : " build\ovl\shc.log
set XX=%1
set XX=%XX:~4%
for %%s in (A:0CE30000 B:0CE38000 C:0CE40000 D:0CE48000 E:0CE50000 F:0CE58000) do call :dcslot %1 %XX% %%s || exit /b 1
exit /b 0
:dcslot
rem %3 is the slot's letter and address; a module with no constants or no work has no C or B to
rem place (lnk's 121)
for /f "tokens=1,2 delims=:" %%a in ("%3") do (set SLOT=%%a& set AT=%%b)
set F=build\ovl\S_PL%2%SLOT%
if exist %F%.BIN del %F%.BIN
"%SHC%\lnk.exe" build\ovl\%1.obj,build\ovl\dcsyms.obj -output=%F%.abs -elf -form=a -start=P,C,D,B(%AT%) -udfcheck >build\ovl\lnk.log 2>&1
findstr /b /c:"**" build\ovl\lnk.log | findstr /v /c:" 121 " && exit /b 1
"%SHC%\elf2bin.exe" %F%.abs %F%.BIN >nul 2>&1 || exit /b 1
del %F%.abs
exit /b 0
:dcsha
rem %1 is a SHA-1 from dcovl.sha1, %2 its file
set P=%2
set P=%P:/=\%
certutil -hashfile %P% SHA1 | findstr /i /c:"%1" >nul || (echo %P% is not the disc's& set /a BAD+=1)
exit /b 0

:dcobj
rem compile build\obj\dc\%1.obj from the first source that has it
for %%d in (src\dc src) do for %%f in (%%d\%1.c %%d\%1.src) do if exist %%f (call :dcone %%f %%d & exit /b)
for %%l in (src\dc\lib src\lib) do for /d %%d in (%%l\*) do for %%f in (%%d\%1.c %%d\%1.src) do if exist %%f (call :dcone %%f %%d & exit /b)
echo no source for build\obj\dc\%1.obj
exit /b 1
:dcone
if /i "%~x1"==".src" (
	"%SHC%\asmsh.exe" %1 -object=build\obj\dc\%~n1.obj %AOPTS% >build\obj\dc\asm.log || (type build\obj\dc\asm.log & exit /b 1)
	exit /b 0
)
set LIB=%2
for %%l in ("%LIB%") do set LIBNAME=%%~nxl
for %%l in ("%LIB%\..") do set LIBDIR=%%~nxl
if exist %~dpn1.options goto dclib
if /i "%LIBDIR%"=="lib" goto dclib
set INC=include
if /i "%LIBNAME%"=="dc" set INC=src\dc,include
"%SHC%\shc.exe" %1 %DCOPTS% -include=%INC% -object=build\obj\dc\%~n1.obj >build\obj\dc\shc.log 2>&1 || (type build\obj\dc\shc.log & exit /b 1)
findstr /c:") : " build\obj\dc\shc.log
exit /b 0
:dclib
rem Sega's: the disc's options for it in src\dc\lib, the module's, its folder's, or else its library's
rem in src\lib, and the headers of both folders; one with inline assembler is compiled to assembler
rem source and assembled
set OPTS=src\lib\%LIBNAME%\options
if exist %LIB%\options set OPTS=%LIB%\options
if exist %~dpn1.options set OPTS=%~dpn1.options
if exist src\dc\lib\%LIBNAME%\%~n1.options set OPTS=src\dc\lib\%LIBNAME%\%~n1.options
set /p OPTS=<%OPTS%
set INC=%LIB%
if exist src\lib\%LIBNAME% if /i not "%LIB%"=="src\lib\%LIBNAME%" set INC=%LIB%,src\lib\%LIBNAME%
findstr /c:"#pragma inline_asm" %1 >nul && goto dclibasm
"%SHC%\shc.exe" %1 %OPTS% -include=%INC%,include,build\orig -object=build\obj\dc\%~n1.obj >build\obj\dc\shc.log 2>&1 || (type build\obj\dc\shc.log & exit /b 1)
findstr /c:") : " build\obj\dc\shc.log
exit /b 0
:dclibasm
"%SHC%\shc.exe" %1 %OPTS% -code=asmcode -include=%INC%,include,build\orig -object=build\tmp\%~n1.src >build\obj\dc\shc.log 2>&1 || (type build\obj\dc\shc.log & exit /b 1)
"%SHC%\asmsh.exe" build\tmp\%~n1.src -object=build\obj\dc\%~n1.obj %AOPTS% >build\obj\dc\asm.log || (type build\obj\dc\asm.log & exit /b 1)
exit /b 0

:fail
popd
exit /b 1
