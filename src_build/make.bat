@echo off
setlocal

set "IN=%~dpn0.cc"
set "OUT=%~dpn0.exe"
set "BASE=%CD:\=/%"
set "FLAGS=-std=c++20 -O3 -fmacro-prefix-map=%BASE%/="
if defined CXX (
  set "CMD=%CXX% %FLAGS% -DCXX="%CXX%" "%IN%" -o "%OUT%""
) else (
  set "CMD=c++ %FLAGS% "%IN%" -o "%OUT%""
)

%CMD%
if errorlevel 1 exit /b %ERRORLEVEL%
"%OUT%" %*
set "EXIT_CODE=%ERRORLEVEL%"
if exist "%OUT%" del "%OUT%"
exit /b %EXIT_CODE%
