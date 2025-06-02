@echo off
set /a i=1,n=51
:begfor
 echo step%i%:
 data_constructor.exe
 first.exe > a.txt
 test.exe > b.txt
 echo;
 echo;
 fc a.txt b.txt
 if errorlevel 1 goto endfor
 set /a i=%i%+1
 if not %i%==%n% goto begfor
:endfor
echo Checking End!
del a.txt
del b.txt
pause