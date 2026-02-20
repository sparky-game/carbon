@echo off
setlocal

set "IN=%~dpn0.cc"
set "OUT=%~dpn0.exe"
set "CXX=c++ -std=c++20 -O3"

%CXX% "%IN%" -o "%OUT%" && "%OUT%" %*
set "EXIT_CODE=%ERRORLEVEL%"
if exist "%OUT%" del "%OUT%"
exit /b %EXIT_CODE%
