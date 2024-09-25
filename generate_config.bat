@echo off
setlocal

if exist config.h (
    echo config.h already exists. No action taken.
) else (
    if exist config-example.h (
        copy config-example.h config.h
        if errorlevel 1 (
            echo Error: Failed to copy config-example.h to config.h
        ) else (
            echo Successfully created config.h from config-example.h
        )
    ) else (
        echo Error: config-example.h not found in the current directory.
    )
)

endlocal
