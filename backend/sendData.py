import serial
import time

# Configure the serial port
port = 'COM13'  # Change this to your COM port (e.g., 'COM1' on Windows)
baud_rate = 115200  # Change this to match your microcontroller's baud rate

# Open the serial port
ser = serial.Serial(port, baudrate=baud_rate, timeout=1)

# Wait for the serial connection to be established
#time.sleep(2)  # Wait for 2 seconds

try:
    # Define an array of unsigned 8-bit integers
    data_to_send = [1, 3, 5, 7, 9, 11 , 13, 13, 13, 13]

    # Pack the array into bytes and send it over serial
    ser.write(bytes(data_to_send))

    # Wait for a brief moment to ensure the microcontroller has time to process the data
    #time.sleep(0.1)

    # Listen for a message from the microcontroller
    received_data = ser.readline().decode('utf-8').strip()
    print("Received message:", received_data)

except KeyboardInterrupt:
    print("\nExiting program.")

finally:
    # Close the serial port
    ser.close()