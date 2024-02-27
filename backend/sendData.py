import serial
import time

# Configure the serial port
port = 'COM8'  # Change this to your COM port (e.g., 'COM1' on Windows)
baud_rate = 115200  # Change this to match your microcontroller's baud rate

# Open the serial port
ser = serial.Serial(port, baudrate=baud_rate, timeout=1)

def protocol_0():
    print("Protocol 0: START")
    data_to_send = [1,3,5,7,9,11,13,0,0,0]  
    ser.write(bytes(data_to_send))

def protocol_1():
    print("Protocol 1: VAC_ACHIEVMENT_TEST_INIT")
    data_to_send = [1,0,1,0,0,1,3,50]  
    ser.write(bytes(data_to_send))

def protocol_2():
    print("Protocol 2: VAC_ACHIEVMENT_TEST")
    data_to_send = [2,0,1,0,0,1,3,0] 
    # ser.write(bytes(data_to_send))

def protocol_3():
    print("Protocol 3: SPECIAL_TEST_INIT")
    data_to_send = [3,0,0,10,0,3,0,50,50]  
    ser.write(bytes(data_to_send))

def protocol_4():
    print("Protocol 4: SPECIAL_TEST")
    data_to_send = [4,0,1,0,0,1,3,0] 
    # ser.write(bytes(data_to_send))

def protocol_5():
    print("Protocol 5: WARM_UP_INIT")
    data_to_send = [5,0,3,0,0,0,0,100]  
    ser.write(bytes(data_to_send))

def protocol_6():
    print("Protocol 6: WARM_UP")
    data_to_send = [6,0,1,0,0,1,3,0] 
    # ser.write(bytes(data_to_send))

def protocol_7():
    print("Protocol 7: LOAD_TEST_INIT")
    data_to_send = [7,0,8,0,0,3,0,100]  
    ser.write(bytes(data_to_send))

def protocol_8():
    print("Protocol 8: LOAD_TEST")
    data_to_send = [8,0,1,0,0,1,3,0]  
    #ser.write(bytes(data_to_send))

def protocol_9():
    print("Protocol 9: OPERATION_TEST_INIT")
    data_to_send = [9,0,4,0,0,3,0,100]  
    ser.write(bytes(data_to_send))

def protocol_10():
    print("Protocol 10: OPERATION_TEST")
    data_to_send = [1,0,1,0,0,1,3,0]  
    # ser.write(bytes(data_to_send))

def protocol_11():
    print("Protocol 11: ULTIMATE_MEASURE_TEST_INIT")
    data_to_send = [11,0,3,0,3,0,0,100,15]  
    ser.write(bytes(data_to_send))

def protocol_12():
    print("Protocol 12: ULTIMATE_MEASURE_TEST")
    data_to_send = [1,0,1,0,0,1,3,0] 
    # ser.write(bytes(data_to_send))

def protocol_13():
    print("Protocol 13: IDLE")
    data_to_send = [1,0,1,0,0,1,3,0]  
    #ser.write(bytes(data_to_send))

def protocol_14():
    print("Protocol 14: FAIL_STATE")
    data_to_send = [1,0,1,0,0,1,3,0]
    # ser.write(bytes(data_to_send))

def protocol_15():
    print("Protocol 15: STOP")
    data_to_send = [1,0,1,0,0,1,3,0]  
    #ser.write(bytes(data_to_send))



# Define a dictionary mapping byte values to protocol functions
protocols = {
    0: protocol_0,
    1: protocol_1,
    2: protocol_2,
    3: protocol_3,
    4: protocol_4,
    5: protocol_5,
    6: protocol_6,
    7: protocol_7,
    8: protocol_8,
    9: protocol_9,
    10: protocol_10,
    11: protocol_11,
    12: protocol_12,
    13: protocol_13,
    14: protocol_14,
    15: protocol_15
}
print("\nStarting program")
try:
    while True:
        # Receive a single byte over serial
        received_byte = ser.read(1)

        if received_byte:
            # Decode the byte and get the protocol function associated with it
            protocol_func = protocols.get(ord(received_byte), None)

            if protocol_func:
                # Execute the protocol function
                protocol_func()
            else:
                print("Unknown protocol for byte:", ord(received_byte))

except KeyboardInterrupt:
    print("\nExiting program.")

finally:
    # Close the serial port
    ser.close()