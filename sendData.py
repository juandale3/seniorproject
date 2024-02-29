import serial
import time

# Configure the serial port
port = 'COM7'  # Change this to your COM port (e.g., 'COM1' on Windows)
baud_rate = 115200  # Change this to match your microcontroller's baud rate

# Open the serial port
ser = serial.Serial(port, baudrate=baud_rate, timeout=1000)

def protocol_0():
    print("Protocol 0: START")
    # data_to_send = [0,1,3,5,7,9,11,13,13,13,11,12,13,14,15,16,17,18,19,20] 
    data_to_send = [0,7,13,11,13,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0] 
    ser.write(bytes(data_to_send))

def protocol_1():
    print("\nProtocol 1: VAC_ACHIEVMENT_TEST_INIT")
    # data_to_send = [1,0,1,0,0,1,3,50]     # actual Default Parameters
    data_to_send = [1,0,0,1,0,1,3,50]       # testing parameters, test time reduced
    ser.write(bytes(data_to_send))

def protocol_2():
    received_data = ser.read(7 - 1)
    print("Protocol 2: VAC_ACHIEVMENT_TEST\t\tReceived data:", [int(byte) for byte in received_data], end="\r")

def protocol_3():
    print("\nProtocol 3: SPECIAL_TEST_INIT")
    # data_to_send = [3,0,0,10,0,3,0,50,50]     # actual Default Parameters
    data_to_send = [3,0,0,1,0,3,0,50,50]        # testing parameters, test time reduced
    ser.write(bytes(data_to_send))
    
def protocol_4():
    received_data = ser.read(8 - 1)
    print("Protocol 4: SPECIAL_TEST\t\tReceived data:", [int(byte) for byte in received_data], end="\r")
    
def protocol_5():
    print("\nProtocol 5: WARM_UP_INIT")
    # data_to_send = [5,0,3,0,0,0,0,100]        # actual Default Parameters
    data_to_send = [5,0,0,1,0,0,0,100]          # testing parameters, test time reduced
    ser.write(bytes(data_to_send))

def protocol_6():
    received_data = ser.read(6 - 1)
    print("Protocol 6: WARM_UP\t\t\tReceived data:", [int(byte) for byte in received_data], end="\r")

def protocol_7():
    print("\nProtocol 7: LOAD_TEST_INIT")
    # data_to_send = [7,0,8,0,0,3,0,100]        # actual Default Parameters
    data_to_send = [7,0,0,1,0,3,0,100]          # testing parameters, test time reduced
    ser.write(bytes(data_to_send))

def protocol_8():
    received_data = ser.read(7 - 1)
    print("Protocol 8: LOAD_TEST\t\t\tReceived data:", [int(byte) for byte in received_data], end='\r')

def protocol_9():
    print("\nProtocol 9: OPERATION_TEST_INIT")
    # data_to_send = [9,0,4,0,0,3,0,100]        # actual Default Parameters
    data_to_send = [9,0,0,1,0,3,0,100]          # testing parameters, test time reduced
    ser.write(bytes(data_to_send))

def protocol_10():
    received_data = ser.read(8 - 1)   
    print("Protocol 10: OPERATION_TEST\t\tReceived data:", [int(byte) for byte in received_data], end='\r')

def protocol_11():
    print("\nProtocol 11: ULTIMATE_MEASURE_TEST_INIT")
    # data_to_send = [11,0,3,0,3,0,0,100,15]    # actual Default Parameters
    data_to_send = [11,0,0,1,3,0,0,100,50,15]      # testing parameters, test time reduced
    ser.write(bytes(data_to_send))

def protocol_12():
    received_data = ser.read(8 - 1)
    print("Protocol 12: ULTIMATE_MEASURE_TEST\tReceived data:", [int(byte) for byte in received_data], end='\r')

def protocol_13():
    received_data = ser.read(8 - 1)
    print("Protocol 13: IDLE\t\tReceived data:", [int(byte) for byte in received_data], end='\r') 

def protocol_14():
    print("\nProtocol 14: FAIL_STATE")

def protocol_15():
    print("Protocol 15: STOP")




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