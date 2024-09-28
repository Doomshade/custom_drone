# Drone Flight Control Unit

_Important note: this is a toy project and should not be used in real drones. Its purpose
is to learn basics about Arduino and how the FCU works in general as it's an interesting topic._

## Installation

### Prerequisities

- [Arduino IDE](https://www.arduino.cc/en/software) installed in the standard directory (either `C:\Program Files\Arduino IDE` or in `C:\Users\%USER%\AppData\Local\Programs\Arduino IDE`).
- [Git](https://git-scm.com/downloads/win) _(optional)_

### How to initialize

First of all, create a directory on your computer and clone the project:

```bat
> git clone https://github.com/Doomshade/drone_fcu.git
```

If you don't have [Git](https://git-scm.com/downloads/win) installed you can just download the [ZIP file](https://github.com/Doomshade/drone_fcu/archive/refs/heads/master.zip) instead.

> [!IMPORTANT]
> Make sure to remove the `-master` suffix from the directory if you downloaded via ZIP file as it must have the same name as the `.ino` file!

Then, `cd` into the directory and run `init.bat`:

```bat
> cd drone_fcu
> init.bat
```

This will generate `config.h` from the `config-example.h` file that you may now
configure as u like. This prevents accidental override of the config if you ever `git pull --rebase`.

This script will also attempt to install needed dependencies:

- Adafruit MPU6050

It will attempt to locate the Arduino IDE in the standard directory and run the
installed `arduino-cli.exe` that comes with the Arduino IDE installation.

### How to run the project

To run the project, run `run.bat`:

```bat
> run.bat
```

This script uses default port `COM3` and board `arduino:avr:uno`.
To configure this simply modify the following variables in the `run.bat` script:

```
REM Set variables for the sketch and port
set "SKETCH_DIR=%CD%"
set "PORT=COM3"
set "FQBN=arduino:avr:uno"
```

You may now send commands to the drone via the serial monitor. List of commands
is located in `cmd.h`. The command list will be listed here someday, trust!