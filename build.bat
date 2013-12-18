@echo off
echo compiling...
gcc main.c net.c utils.c -o bin\main.exe -lws2_32
echo done.
pause>nul