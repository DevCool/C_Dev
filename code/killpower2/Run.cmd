@echo off
timeout 5 > NUL
if "%1"=="a" goto login
if "%1"=="b" goto talker
if "%1"=="c" goto shutdown
exit

:login
start cmd.exe /k client.exe 127.0.0.1 Test . nipTuck88!@#
exit

:talker
start cmd.exe /k client.exe -t 127.0.0.1 "Hello world this is a test."
exit

:shutdown
start cmd.exe /k client.exe -p "C:\Windows\System32\Notepad.exe"
exit
