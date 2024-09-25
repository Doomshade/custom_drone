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

REM Set variables for the sketch and port
set "SKETCH_DIR=%CD%"
set "PORT=COM3"
set "FQBN=arduino:avr:uno"

"%ARDUINO_CLI%" upload -p %PORT% --fqbn %FQBN% "%SKETCH_DIR%"

if errorlevel 1 (
    echo Error: Failed to upload the sketch.
    pause
    goto :EOF
)

echo Sketch uploaded successfully.


REM Run the serial monitor
echo Starting serial monitor. Press Ctrl+C to exit.
"%ARDUINO_CLI%" monitor -p %PORT%

endlocal

