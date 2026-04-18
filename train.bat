@echo off
REM Activate enviroment
echo Activating enviroment
cd /d "C:\Users\YourUsername\OneDrive\Documents\RLBot\GigaLearnCPP-Leak"
call "gigalearn_env\Scripts\activate.bat"

REM Change to the build directory
cd build

REM echo Configuring...
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=RelWithDebInfo -DTorch_DIR="C:/Users/yiric/OneDrive/Documents/GigaLearnCPP-Leak/GigaLearnCPP/libtorch/share/cmake/Torch"
echo Building...
cmake --build . --config RelWithDebInfo --target GigaLearnBot

REM Move to the RelWithDebInfo folder and run the bot (training mode)
cd RelWithDebInfo
echo Training...
GigaLearnBot.exe
pause

REM Put this into your /build folder by the way, you can make a windows shortcut and drag it to your desktop folder for easy access.
