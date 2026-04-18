@echo off
REM Activate enviroment
echo Activating enviroment
cd /d "C:\Users\YourUsername\OneDrive\Documents\RLBot\GigaLearnCPP-Leak"
call "gigalearn_env\Scripts\activate.bat"

REM Change to the build directory
cd /d C:\Users\YourUsername\OneDrive\Documents\GigaLearnCPP-Leak\build\RelWithDebInfo
echo Rendering...
GigaLearnBot.exe --render
pause

REM Put this into your /build folder by the way, you can make a windows shortcut and drag it to your desktop folder for easy access.
REM ️‼️By the way, as you can see, I didn't put a configuring/build line into this file because I am assuming you already finished building while training.
R️EM ‼️👀IMPORTANT: You need to start up the Visualizer.bat file too, as that renders it, this file just starts up a game to visualize, but doesn't render it.
