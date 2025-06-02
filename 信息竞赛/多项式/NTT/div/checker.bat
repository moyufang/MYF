@echo off
set /a i=1,n=51
:begfor
 echo step%i%:
 div_data.exe
 div.exe
 echo;
 echo right;
 echo;
 set /a i=%i%+1
 if not %i%==%n% goto begfor
:endfor
echo Checking End!
pause