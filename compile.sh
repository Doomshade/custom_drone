# Set variables for the sketch and port
FQBN="arduino:avr:nano"

# Compile the sketch
arduino-cli compile -e --fqbn "$FQBN"

if [ $? -ne 0 ]; then
    echo "Error: Failed to compile the sketch."
    exit 1
fi

echo "Compilation successful."
