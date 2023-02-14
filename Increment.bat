echo off
setlocal enabledelayedexpansion

set filepath=%1
set filename=%~n1%~x1

if exist "%filepath%.temp" del "%filepath%.temp"
   
for /f "tokens=1,2* delims=:" %%Q in ('type "%filepath%" ^| findstr /n ".*"') do (
   
    if "%%R"=="" echo. >> "%filepath%.temp"
     
    set incflag=0
     
    for /f "tokens=1-3 delims= " %%A in ("%%R") do (
     
        if "%%B"==%2 set incflag=1
         
        if !incflag! equ 1 (
        set /a num=%%C+1
           
        echo Incrementing %%B from %%C to !num!
           
        echo %%A %%B !num! >>"%filepath%.temp"
         
        ) else (

        echo %%R >> "%filepath%.temp"
        )
    )
)

del "%filepath%"
   
ren "%filepath%.temp" "%filename%"