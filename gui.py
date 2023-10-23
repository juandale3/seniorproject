import serial
from tkinter import *
import tkinter as tk 
#last update by manny
ser = serial.Serial('COM3', baudrate = 9600, timeout = 1)

def turnOnLED():
    ser.write(b'o')

def turnOffLED(): 
    ser.write(b'x')

def on_closing():
    ser.close()
    root.destroy()

# creating tkinter window 
root = Tk() 
root.title('Blink GUI')

btn_On= tk.Button(root, text="Turn On", command=turnOnLED)
btn_On.grid(row=0, column=0)

btn_Off = tk.Button(root, text="Turn Off", command=turnOffLED)
btn_Off.grid(row=0, column=1)

root.geometry("350x350")
root.protocol("WM_DELETE_WINDOW", on_closing)
root.mainloop()