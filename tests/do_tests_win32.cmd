:: this compiles and runs the test suite under Visual Studio 2008
::@echo off
call "C:\Program Files\Microsoft Visual Studio 9.0\VC\bin\vcvars32.bat" > vc.out
::call "C:\Program Files\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat" > vc.out
set "COMPILE=cl.exe /I ..\src /EHsc /nologo"
echo compiling...
%COMPILE% tdiff.cpp > compile.out
::for %%f in (test*.c) do %COMPILE% /Tp %%f >> compile.out
for %%f in (test*.c) do %COMPILE% /Tc %%f >> compile.out
echo running tests...
for %%f in (test*.exe) do %%f > %%~nf.out
echo scanning for failures...
for %%f in (test*.out) do tdiff %%f %%~nf.ans
echo tests completed
::for %%f in (test*.out test*.obj test*.exe vc.out compile.out tdiff.obj tdiff.exe) do del %%f
pause
