@echo off
set /a i=1,n=51
set /a t=%time:~3,2%*10000+%time:~6,2%*100+%time:~9,2%
echo time:%t%
:begfor
 echo step%i%:
 set /a t=%t%+1
 echo %t% > seed.txt
 data_constructor.exe
 first.exe > a.txt
 test.exe > b.txt
 echo;
 type a.txt
 echo;
 fc a.txt b.txt
 if errorlevel 1 goto endfor
 set /a i=%i%+1
 if not %i%==%n% goto begfor
:endfor
echo Checking End!
pause