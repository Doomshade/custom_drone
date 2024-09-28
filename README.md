# Drone Flight Control Unit

> [!IMPORTANT]
> This is a toy project and should not be used in real drones. Its purpose
is for me _(Doomshade)_ to learn basics about Arduino, and in general how the FCU works as it's an interesting topic to me.

## Installation

### Prerequisities

- [Arduino IDE](https://www.arduino.cc/en/software) installed in the standard directory (either `C:\Program Files\Arduino IDE` or in `C:\Users\%USER%\AppData\Local\Programs\Arduino IDE`).
- [Git](https://git-scm.com/downloads/win) _(optional)_

### Project initialization

Clone the project:

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

> [!NOTE]
> This script will also attempt to install needed dependencies:
>
> - Adafruit MPU6050

### How to run the project

To run the project you will need your Arduino connected to your computed via USB, and then run `run.bat`:

```bat
> run.bat
```

This script uses default port `COM3` and board `arduino:avr:uno`.


> [!TIP]
> To configure this simply modify the following variables in the `run.bat` script:
> ```
> REM Set variables for the sketch and port
> set "SKETCH_DIR=%CD%"
> set "PORT=COM3"
> set "FQBN=arduino:avr:uno"
> ```

You may now send commands to the drone via the serial monitor.

List of commands:

- `esc arm`
  - Arms the ESC
- `mpu dbg`
  - Starts printing debug information of the MPU readings
  - The debug rate can be configured in `config.h`
- `mpu cal`
  - Calibrates the MPU (typically the gyro)
- `motor e`
  - Enables all motors
- `motor d`
  - Disables all motors
- `motor t`
  - Tests the functionality of motors
  - This expects the ESC to be armed
- `motor e <1,4>`
  - Enables a single motor
- `motor d <1,4>`
  - Disables a single motor
- `motor s <0,100>`
  - Sets the % speed of motors