import serial
import time
import saveFile
import customtkinter
from datetime import datetime
import struct
import serial.tools.list_ports

initDataFile = ".\data\initData.txt"

textbox = customtkinter.CTkTextbox
# Configure the serial port
port = 'COM1'  # Change this to your COM port
baud_rate = 115200  # Change this to match your microcontroller's baud rate
serialData =""
textbox_data = ""
ser = serial.Serial(port, baudrate=baud_rate, timeout=100)

# Function to find the COM port of the STM32 device
def find_stm32_port(title_textbox, textbox):
    # Enumerate all available COM ports
    ports = serial.tools.list_ports.comports()
    title_textbox.delete("1.0",'end')
    title_textbox.insert('end', "Establishing connection!\n")
    # Iterate through the list of ports
    for port in ports:
        try:
            # Check if the port description matches "STMicroelectronics STLink Virtual COM Port"
            if "STMicroelectronics STLink Virtual COM Port" in port.description or "USB Serial Port" in port.description:
                # Found the STLink Virtual COM Port
                textbox.insert('end', "\nSTLink Virtual COM Port found on:", port.device)
                # Close the serial connection
                ser.close()
                # Return the COM port
                return port.device
        except serial.SerialException:
            pass
    # Return None if STM32 device not found
    textbox.insert('end', "\nSTM32 Not Found\n")
    return None

def writeInitData(file,data):
    with open(file,'a') as initDataFile:
        initDataFile.write(data + "\n")
        initDataFile.close()

def protocol_0(title_textbox, textbox, ser):
    data_to_send = [1,3,5,7,9,11,13,0,0,0,0,0,0,0,0,0,0,0,0,20] 
    # data_to_send = [0,7,13,11,13,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0] 
    ser.write(bytes(data_to_send))
    title_textbox.delete("1.0",'end')
    title_textbox.insert('end', "Starting Pump Test!\n")
    textbox.insert('end', "Protocol 0: START\n")


def protocol_1(title_textbox, textbox, ser):
    
    textbox.insert('end', "\nProtocol 1: VAC_ACHIEVMENT_TEST_INIT\n")
    # data_to_send = [1,0,1,0,0,1,3,50]     # actual Default Parameters
    data_to_send = [1,0,0,1,0,1,3,50]       # testing parameters, test time reduced
    ser.write(bytes(data_to_send))

def protocol_2(title_textbox, textbox, ser):
    # Read 12 bytes of data (excluding start byte)
    received_data = ser.read(13 - 1)

    # Update title of the textbox
    title_textbox.delete('1.0','end')
    title_textbox.insert('end',"Vacuum Achievement Test")

    # Interpret received data
    data = struct.unpack('<BBBBff', received_data)  # Assuming byte order is little-endian
    pump, hours, minutes, seconds, vacuum_pressure, temperature = data

    textbox.delete("1.0","end")
    # Current Time
    now = datetime.now()
    formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")
    textbox.insert('end',f"Start Time: {formatted_now}\n\n")

    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Vacuum Pressure: {vacuum_pressure} Torr\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} F\n\n")

def protocol_3(title_textbox, textbox, ser):
    title_textbox.delete('1.0','end')
    title_textbox.insert('end','Initiating Special Test')
    textbox.insert('end', "\nProtocol 3: SPECIAL_TEST_INIT\n")
    # data_to_send = [3,0,0,10,0,3,0,50,50]     # actual Default Parameters
    data_to_send = [3,0,0,1,0,3,0,50,50]        # testing parameters, test time reduced
    ser.write(bytes(data_to_send))
    
def protocol_4(title_textbox, textbox, ser):
    # Read 16 bytes of data (excluding start byte)
    received_data = ser.read(17 - 1)

    # Update title of the textbox
    title_textbox.delete('1.0','end')
    textbox.delete('1.0','end')
    title_textbox.insert('end','SPECIAL TEST\n')
    
    # Interpret received data
    data = struct.unpack('<BBBBfff', received_data)  # Assuming byte order is little-endian
    pump, hours, minutes, seconds, vacuum_pressure, temperature, flow_rate = data

    #   Current Time
    now = datetime.now()
    formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")
    textbox.insert('end',f"Start Time: {formatted_now}\n\n")

    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Vacuum Pressure: {vacuum_pressure} Torr\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} F\n\n")
    textbox.insert('end',f"Current Flow rate: {flow_rate} L/min \n\n")


def protocol_5(title_textbox, textbox, ser):
    textbox.insert('end', "\nProtocol 5: WARM_UP_INIT\n")
    # data_to_send = [5,0,3,0,0,0,0,100]        # actual Default Parameters
    data_to_send = [5,0,0,1,0,0,0,100]          # testing parameters, test time reduced
    ser.write(bytes(data_to_send))

def protocol_6(title_textbox, textbox, ser):
    # Read 8 bytes of data (excluding start byte)
    received_data = ser.read(9 - 1)

    # Update title of the textbox
    textbox.delete('1.0','end')
    title_textbox.delete('1.0','end')
    title_textbox.insert('end','WARM UP TEST')
    
    # Interpret received data
    data = struct.unpack('<BBBBf', received_data)  # Assuming byte order is little-endian
    pump, hours, minutes, seconds, temperature = data
    
    #   Current Time
    now = datetime.now()
    formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")
    textbox.insert('end',f"Start Time: {formatted_now}\n\n")

    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} F\n\n")

def protocol_7(title_textbox, textbox, ser):
    textbox.insert('end', "\nProtocol 7: LOAD_TEST_INIT\n")
    # data_to_send = [7,0,8,0,0,3,0,100]        # actual Default Parameters
    data_to_send = [7,0,0,1,0,3,0,100]          # testing parameters, test time reduced
    ser.write(bytes(data_to_send))

def protocol_8(title_textbox, textbox, ser):
    # Read 12 bytes of data (excluding start byte)
    received_data = ser.read(13 - 1)

    # Update title of the textbox
    title_textbox.delete('1.0','end')
    textbox.delete('1.0','end')
    title_textbox.insert('end','LOAD TEST')
    
    # Interpret received data
    data = struct.unpack('<BBBBff', received_data)  # Assuming byte order is little-endian
    pump, hours, minutes, seconds, temperature, flow_rate = data

    # Current Time
    now = datetime.now()
    formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")
    textbox.insert('end',f"Start Time: {formatted_now}\n\n")

    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} F\n\n")
    textbox.insert('end',f"Current Flow rate: {flow_rate} L/min \n\n")


def protocol_9(title_textbox, textbox, ser):
    textbox.insert('end', "\nProtocol 9: OPERATION_TEST_INIT\n")
    # data_to_send = [9,0,4,0,0,3,0,100]        # actual Default Parameters
    data_to_send = [9,0,0,1,0,3,0,100]          # testing parameters, test time reduced
    ser.write(bytes(data_to_send))

def protocol_10(title_textbox, textbox, ser):
    # Read 16 bytes of data (excluding start byte)
    received_data = ser.read(17 - 1)

    # Update title of the textbox  
    title_textbox.delete('1.0','end')
    textbox.delete('1.0','end')
    title_textbox.insert('end',"Operation Test")
    
    # Interpret received data
    data = struct.unpack('<BBBBfff', received_data)  # Assuming byte order is little-endian
    pump, hours, minutes, seconds, vacuum_pressure, temperature, flow_rate = data

    # Current Time
    now = datetime.now() 
    formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")
    textbox.insert('end',f"Start Time: {formatted_now}\n\n")

    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Vacuum Pressure: {vacuum_pressure} Torr\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} F\n\n")
    textbox.insert('end',f"Current Flow rate: {flow_rate} L/min \n\n")

def protocol_11(title_textbox, textbox, ser):
    textbox.insert('end', "\nProtocol 11: ULTIMATE_MEASURE_TEST_INIT\n")
    # data_to_send = [11,0,3,0,3,0,0,100,15]    # actual Default Parameters
    data_to_send = [11,0,0,1,3,0,0,100,50,15]      # testing parameters, test time reduced
    ser.write(bytes(data_to_send))

def protocol_12(title_textbox, textbox, ser):
    # Read 16 bytes of data (excluding start byte)
    received_data = ser.read(17 - 1)

    # Update title of the textbox
    title_textbox.delete('1.0','end')
    textbox.delete('1.0','end')
    title_textbox.insert('end','Ulimate Pressure Test')

    # Interpret received data
    data = struct.unpack('<BBBBfff', received_data)  # Assuming byte order is little-endian
    pump, hours, minutes, seconds, vacuum_pressure, temperature, flow_rate = data

    #Current Time
    now = datetime.now()    
    formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")
    textbox.insert('end',f"Start Time: {formatted_now}\n\n")
    
    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Vacuum Pressure: {vacuum_pressure} Torr\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} F\n\n")
    textbox.insert('end',f"Current Flow rate: {flow_rate} L/min \n\n")

def protocol_13(title_textbox,textbox, ser):
    received_data = ser.read(17 - 1)
    textbox.insert('end', "Protocol 13: IDLE\t\tReceived data: {}\n".format([int(byte) for byte in received_data])) 
    # # Read 16 bytes of data (excluding start byte)
    # received_data = ser.read(17 - 1)

    # # Update title of the textbox
    # title_textbox.delete('1.0','end')
    # textbox.delete('1.0','end')
    # title_textbox.insert('end','Idle State\n')
    
    # # Interpret received data
    # data = struct.unpack('<BBBBfff', received_data)  # Assuming byte order is little-endian
    # pump, hours, minutes, seconds, vacuum_pressure, temperature, flow_rate = data

    # #   Current Time
    # now = datetime.now()
    # formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")
    # textbox.insert('end',f"Start Time: {formatted_now}\n\n")

    # textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    # textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    # textbox.insert('end',f"Vacuum Pressure: {vacuum_pressure} Torr\n\n")
    # textbox.insert('end',f"Current Pump Temperature: {temperature} F\n\n")
    # textbox.insert('end',f"Current Flow rate: {flow_rate} L/min \n\n")

def protocol_14(title_textbox, textbox, ser):
    textbox.insert('end', "\nProtocol 14: FAIL_STATE\n")
    # # Read 16 bytes of data (excluding start byte)
    # received_data = ser.read(17 - 1)

    # # Update title of the textbox
    # title_textbox.delete('1.0','end')
    # textbox.delete('1.0','end')
    # title_textbox.insert('end','Failed State\n')
    
    # # Interpret received data
    # data = struct.unpack('<BBBBfff', received_data)  # Assuming byte order is little-endian
    # pump, hours, minutes, seconds, vacuum_pressure, temperature, flow_rate = data

    # #   Current Time
    # now = datetime.now()
    # formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")
    # textbox.insert('end',f"Start Time: {formatted_now}\n\n")

    # textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    # textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    # textbox.insert('end',f"Vacuum Pressure: {vacuum_pressure} Torr\n\n")
    # textbox.insert('end',f"Current Pump Temperature: {temperature} F\n\n")
    # textbox.insert('end',f"Current Flow rate: {flow_rate} L/min \n\n")

def protocol_15(title_textbox, textbox, ser):
    title_textbox.delete('1.0', 'end')
    title_textbox.insert("Pump Test have completely Stopped")
    textbox.insert('end', "Protocol 15: STOP\n")




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


