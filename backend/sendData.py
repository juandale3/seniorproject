import serial
import time
import saveFile
import customtkinter
from datetime import datetime
import struct
import serial.tools.list_ports
import stateData

#initDataFile = ".\data\initData.txt"
PORT=''
textbox = customtkinter.CTkTextbox
# Configure the serial port
port = 'COM14'  # Change this to your COM port
baud_rate = 115200  # Change this to match your microcontroller's baud rate
serialData =""
textbox_data = ""
#ser = serial.Serial(port, baudrate=baud_rate, timeout=100)

pump = hours = minutes = seconds = 0
vacuum_pressure = temperature = flow_rate = 0.0 
#   Current Time
now = datetime.now()
formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")


# Function to find the COM port of the STM32 device

def find_stm32_port(title_textbox, label,start_main_button):
    ports = serial.tools.list_ports.comports()
    title_textbox.delete("1.0", 'end')
    label.configure(text="Establishing connection!\n")  # Changed from config to configure
    for port in ports:
        if "STMicroelectronics STLink Virtual COM Port" in port.description or "USB Serial Port" in port.description:
            title_textbox.insert("end","Connection Successful!")
            label.configure(text=f"\nSTLink Virtual COM Port found on: {port.device}",text_color = '#90EE90')  # Changed from config to configure
            stateData.db['stmPORT'] = port.device
            stateData.db['portConnection'] = 1
            start_main_button.configure(state=customtkinter.NORMAL)
            try:
                ser = serial.Serial(port.device)
                ser.close()
            except serial.SerialException:
                pass
            print(f"Using Port: {port.device}")

            return port.device
    title_textbox.insert("end","Connection Failed")
    start_main_button.configure(state=customtkinter.DISABLED)

    label.configure(text="\nSTM32 Not Found\n", text_color="#FF6347")  # Changed from config to configure
    return None

def processData():
    global pump, hours, minutes, seconds, vacuum_pressure, temperature, flow_rate
    # Read 16 bytes of data (excluding start byte)
    received_data = ser.read(17 - 1)
    # Interpret received data
    data = struct.unpack('<BBBBfff', received_data)  # Assuming byte order is little-endian
    pump, hours, minutes, seconds, vacuum_pressure, temperature, flow_rate = data
    print(data)

def writeInitData(file,data):
    with open(file,'a') as initDataFile:
        initDataFile.write(data + "\n")
        initDataFile.close()

def protocol_0(title_textbox, textbox, ser):
    stateData.db["currentState"] = 0
    #data_to_send = [1,3,5,7,9,11,13,0,0,0,0,0,0,0,0,0,0,0,0,20] 
    data_to_send = [13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20] 
    ser.write(bytes(data_to_send))
    textbox.insert('end', "Protocol 0: START\n")

def protocol_1(title_textbox, textbox, ser):
    stateData.db["currentState"] = 1
    # data_to_send = [1,0,1,0,0,1,3,50]     # actual Default Parameters
    data_to_send = [1,0,0,1,0,1,3,50]       # testing parameters, test time reduced
    ser.write(bytes(data_to_send))
    textbox.insert('end', "\nProtocol 1: VAC_ACHIEVMENT_TEST_INIT\n")
    global now, formatted_now
    now = datetime.now()
    formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")
    
def protocol_2(title_textbox, textbox, ser):
    processData()
    # Update title of the textbox
    title = "Vacuum Achievement Test"
    stateData.db["currentState"] = 1
    stateData.db["protocols"][0][title] = title

    title_textbox.delete('1.0','end')
    title_textbox.insert('end',title)
    textbox.delete("1.0","end")
    textbox.insert('end',f"Start Time: {formatted_now}\n\n")
    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Vacuum Pressure: {vacuum_pressure} Torr\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} C\n\n")

    print(f"current test: {stateData.db["protocols"][0][title]}")
def protocol_3(title_textbox, textbox, ser):
    # data_to_send = [3,0,0,10,0,3,0,50,50]     # actual Default Parameters
    data_to_send = [3,0,0,1,0,3,0,50,50]        # testing parameters, test time reduced
    ser.write(bytes(data_to_send))
    title_textbox.delete('1.0','end')
    title_textbox.insert('end','Initiating Special Test')
    textbox.insert('end', "\nProtocol 3: SPECIAL_TEST_INIT\n")
    global now, formatted_now
    now = datetime.now()
    formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")
    
def protocol_4(title_textbox, textbox, ser):
    processData()
    # Update title of the textbox
    title_textbox.delete('1.0','end')
    textbox.delete('1.0','end')
    title_textbox.insert('end','SPECIAL TEST\n')
    textbox.insert('end',f"Start Time: {formatted_now}\n\n")
    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Vacuum Pressure: {vacuum_pressure} Torr\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} C\n\n")
    textbox.insert('end',f"Current Flow rate: {flow_rate} L/min \n\n")
    

def protocol_5(title_textbox, textbox, ser):
    # data_to_send = [5,0,3,0,0,0,0,100]        # actual Default Parameters
    data_to_send = [5,0,0,1,0,0,0,100]          # testing parameters, test time reduced
    ser.write(bytes(data_to_send))
    textbox.insert('end', "\nProtocol 5: WARM_UP_INIT\n")
    global now, formatted_now
    now = datetime.now()
    formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")

def protocol_6(title_textbox, textbox, ser):
    processData()
   # Update title of the textbox
    textbox.delete('1.0','end')
    title_textbox.delete('1.0','end')
    title_textbox.insert('end','WARM UP TEST')
    textbox.insert('end',f"Start Time: {formatted_now}\n\n")
    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} C\n\n")

def protocol_7(title_textbox, textbox, ser):
    # data_to_send = [7,0,8,0,0,3,0,100]        # actual Default Parameters
    data_to_send = [7,0,0,1,0,3,0,100]          # testing parameters, test time reduced
    ser.write(bytes(data_to_send))
    textbox.insert('end', "\nProtocol 7: LOAD_TEST_INIT\n")
    global now, formatted_now
    now = datetime.now()
    formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")

def protocol_8(title_textbox, textbox, ser):
    processData()
    # Update title of the textbox
    title_textbox.delete('1.0','end')
    textbox.delete('1.0','end')
    title_textbox.insert('end','LOAD TEST')
    textbox.insert('end',f"Start Time: {formatted_now}\n\n")
    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} C\n\n")
    textbox.insert('end',f"Current Flow rate: {flow_rate} L/min \n\n")


def protocol_9(title_textbox, textbox, ser):
    # data_to_send = [9,0,4,0,0,3,0,100]        # actual Default Parameters
    data_to_send = [9,0,0,1,0,3,0,100]          # testing parameters, test time reduced
    ser.write(bytes(data_to_send))
    textbox.insert('end', "\nProtocol 9: OPERATION_TEST_INIT\n")
    global now, formatted_now
    now = datetime.now()
    formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")

def protocol_10(title_textbox, textbox, ser):
    processData()
    # Update title of the textbox  
    title_textbox.delete('1.0','end')
    textbox.delete('1.0','end')
    title_textbox.insert('end',"Operation Test")
    textbox.insert('end',f"Start Time: {formatted_now}\n\n")
    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Vacuum Pressure: {vacuum_pressure} Torr\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} C\n\n")
    textbox.insert('end',f"Current Flow rate: {flow_rate} L/min \n\n")

def protocol_11(title_textbox, textbox, ser):
    # data_to_send = [11,0,3,0,3,0,0,100,15]    # actual Default Parameters
    data_to_send = [11,0,0,1,3,0,0,100,50,15]      # testing parameters, test time reduced
    ser.write(bytes(data_to_send))
    textbox.insert('end', "\nProtocol 11: ULTIMATE_MEASURE_TEST_INIT\n")
    global now, formatted_now
    now = datetime.now()
    formatted_now = now.strftime("%Y-%m-%d   %I:%M:%S %p")

def protocol_12(title_textbox, textbox, ser):
    processData()
    # Update title of the textbox
    title_textbox.delete('1.0','end')
    textbox.delete('1.0','end')
    title_textbox.insert('end','Ulimate Pressure Test')
    textbox.insert('end',f"Start Time: {formatted_now}\n\n")
    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Vacuum Pressure: {vacuum_pressure} Torr\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} C\n\n")
    textbox.insert('end',f"Current Flow rate: {flow_rate} L/min \n\n")

def protocol_13(title_textbox,textbox, ser):
    processData()
    # Update title of the textbox
    title_textbox.delete('1.0','end')
    textbox.delete('1.0','end')
    title_textbox.insert('end','IDLE')
    textbox.insert('end',f"Current Pump: {pump + 1}\n\n")
    textbox.insert('end', f"Time: {hours:02d}:{minutes:02d}:{seconds:02d}\n\n")
    textbox.insert('end',f"Vacuum Pressure: {vacuum_pressure} Torr\n\n")
    textbox.insert('end',f"Current Pump Temperature: {temperature} C\n\n")
    textbox.insert('end',f"Current Flow rate: {flow_rate} L/min \n\n")

def protocol_14(title_textbox, textbox, ser):
    textbox.insert('end', "\nProtocol 14: FAIL_STATE\n")

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


