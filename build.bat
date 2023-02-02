@echo off
@REM tasklist | find /i "%2" && taskkill /im notepad.exe /F || rem
@REM taskkill -im %2 -f
windres vrcw.rc -O coff -o vrcw.res
gcc -O2 -o %2 %1 vrcw.res   