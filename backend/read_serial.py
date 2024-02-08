import serial
import time

# Setup serial connection
ser = serial.Serial('COM9', 115200, timeout=1)

# Flush any remaining input buffer to start fresh
ser.flushInput()

try:
    while True:
        if ser.in_waiting > 0:
            serialData = ser.readline()  # Read data as raw bytes
            print(serialData)  # This will print raw byte data
        time.sleep(0.1)  # A short delay to prevent overwhelming your CPU
except KeyboardInterrupt:
    print("Program terminated by user")
finally:
    ser.close()  # Ensure the serial connection is closed on exit
