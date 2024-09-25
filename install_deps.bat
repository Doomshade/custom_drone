@echo off
setlocal

REM Find arduino-cli.exe
set "IDE_PATH="
if exist "C:\Program Files\Arduino IDE" (
    set "IDE_PATH=C:\Program Files\Arduino IDE"
) else if exist "%LOCALAPPDATA%\Programs\Arduino IDE" (
    set "IDE_PATH=%LOCALAPPDATA%\Programs\Arduino IDE"
)

if not defined IDE_PATH (
    echo Error: Arduino IDE installation not found.
    pause
    goto :EOF
)

set "ARDUINO_CLI=%IDE_PATH%\resources\app\lib\backend\resources\arduino-cli.exe"

if not exist "%ARDUINO_CLI%" (
    echo Error: arduino-cli.exe not found at the expected location.
    pause
    goto :EOF
)

REM Install Adafruit_MPU6050 library and its dependencies
echo Installing Adafruit_MPU6050 library and its dependencies...
"%ARDUINO_CLI%" lib install "Adafruit MPU6050"

if errorlevel 1 (
    echo Error: Failed to install Adafruit_MPU6050 library.
) else (
    echo Successfully installed Adafruit_MPU6050 library and its dependencies.
)

endlocal
