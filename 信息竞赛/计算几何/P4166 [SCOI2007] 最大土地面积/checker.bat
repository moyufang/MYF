@echo off
set /a i=1,n=50+1
:begfor
 echo step%i%:
 data_constructor.exe
 first.exe > a.txt
 test.exe > b.txt
 echo;
 type a.txt
 echo;
 fc a.txt b.txt
 if errorlevel 1 goto Endfor
 set /a i=%i%+1
 if not %i%==%n% goto begfor
:endfor
echo Checking End!
del a.txt
del b.txt
:Endfor
pause