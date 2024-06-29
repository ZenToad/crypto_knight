@echo off
setlocal enableextensions enabledelayedexpansion
REM
set RAYLIB_LIB=..\externals\raylib\build\raylib\Debug
set target_include=-I..\externals\raylib\build\raylib\include\ -I..\externals\raygui\src -I..\externals\raygui\examples\styles -I..\externals\stb -I..\prototypes\proto1 -I..\include
set target_src=
set target_lib=/libpath:%RAYLIB_LIB% raylib.lib winmm.lib gdi32.lib opengl32.lib user32.lib kernel32.lib shell32.lib /NODEFAULTLIB:libcmt
set target_flags=
set compiler=cl
set configuration=debug
REM  set verbose=/VERBOSE
set verbose=
REM Get the full path to the target source file

set target_src=%1

rem Extract the filename without the path
for %%f in ("!target_src!") do set filename=%%~nf

rem Append .exe extension
set target_exe=!filename!.exe

set target_src=..\!target_src!
echo Target Source: ..\!target_src!
echo Target Executable: !newname!

if not exist bin mkdir bin

REM  echo -------------------------------------------------
REM  echo - Compiler:      %compiler%
REM  echo - Include:       %include_dirs%
REM  echo - Configuration: %configuration%
REM  echo - Target C:      %target%.c
REM  echo - Output:        bin\%target_exe%
REM  echo -------------------------------------------------

REM MT for statically linked CRT, MD for dynamically linked CRT
set win_runtime_lib=MDd
set common_c=!target_c! !target_tests! !target_src! /Fe!target_exe! -nologo !target_flags! -FC -EHa- !target_include!
set common_l=!verbose! !target_lib!
echo !common_c!
echo !common_l!

echo.
echo Compiling...
pushd bin
    cl !common_c! -!win_runtime_lib! /Zi /link !common_l!
    if "%ERRORLEVEL%" EQU "0" (
        goto good
    )
    if "%ERRORLEVEL%" NEQ "0" (
        goto bad
    )
    :good
        echo.
        xcopy !target_exe! ..\build /i /y
        echo Running !target_exe!...
        echo.
        !target_exe!
        goto done
    :bad
        echo FAILED
        goto done
    :done

popd

:end

endlocal

