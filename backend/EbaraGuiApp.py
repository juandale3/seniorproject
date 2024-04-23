import customtkinter

import serial
import time
import sendData
import saveFile
import threading
import stateData
from tkinter import PhotoImage

#from sendData import *



#customtkinter.set_appearance_mode("System")  # Modes: "System" (standard), "Dark", "Light"
#customtkinter.set_default_color_theme("blue")

class App(customtkinter.CTk):
    WIDTH = 1100
    HEIGHT = 600

    def __init__(self):
        super().__init__()
        self.initWindow()

        self.title("Semi-Automated Pump Testing Station")
        self.geometry(f"{App.WIDTH}x{App.HEIGHT}")

        self.disabled_button_color = "#2B3E50"
        self.start_button_color = "green"
        self.disabled_text_color = "#FFFFFF"
        # Sidebar frame with widgets
        # Darker shade for the sidebar
        self.sidebar_frame = customtkinter.CTkFrame(self, width=180, corner_radius=0, fg_color="gray20")
        self.sidebar_frame.grid(row=0, column=0, sticky="nswe")
        self.sidebar_frame.grid_rowconfigure(4, weight=1)

        
        self.my_image = PhotoImage(file="ebara_logo.png")  # Replace with your image file path
        # Create a label to display the image
        self.image_label = customtkinter.CTkLabel(self, image=self.my_image, text="")
        self.image_label.grid(row=0, column=0, pady=10,sticky="n")



        self.sidebar_button_5 = customtkinter.CTkButton(self.sidebar_frame, text="Files" ,command="")
        self.sidebar_button_5.grid(row=5 ,column=0, pady=10, padx=20)

        

        # Main content area
        # Lighter shade for the main content area
        self.main_content = customtkinter.CTkFrame(self, fg_color="gray15")
        self.main_content.grid(row=0, column=1, sticky="nsew")
        self.grid_columnconfigure(1, weight=1)
        self.grid_rowconfigure(0, weight=1)
        self.show_content1()



        
    def clear_main_content(self):
        # Clear the main content area
        for widget in self.main_content.winfo_children():
            widget.destroy()

    def initWindow(self):
        self.secondary_window = customtkinter.CTkToplevel(self)
        self.secondary_window.title("Secondary Window")
        self.secondary_window.geometry("400x500")
        self.secondary_window.transient(self)  # Set the parent of the secondary window
        self.secondary_window.grab_set()  # Redirect all events to this window

        # Configure grid weights
        self.secondary_window.grid_columnconfigure(0, weight=1)
        self.secondary_window.grid_columnconfigure(2, weight=1)
        self.secondary_window.grid_rowconfigure(0, weight=1)
        self.secondary_window.grid_rowconfigure(3, weight=1)

        # Load the image properly for PhotoImage in customtkinter
        # Note: PhotoImage should be imported from tkinter
        from tkinter import PhotoImage
        self.my_image = PhotoImage(file="ebara_logo.png")  # Correct file path necessary

        # Create a label to display the image in the secondary window
        self.image_label = customtkinter.CTkLabel(self.secondary_window, image=self.my_image, text="")
        self.image_label.grid(row=0, column=1, pady=10, sticky="n")

        # Initialize a CTkTextbox
        self.init_textbox = customtkinter.CTkTextbox(
            self.secondary_window, height=40, width=200, text_color="white", activate_scrollbars=False
        )
        # Place the textbox in the center, spanning the width of the window using grid
        self.init_textbox.grid(row=1, column=1, padx=10, pady=10)
        

        # Adding a label centered
        label = customtkinter.CTkLabel(self.secondary_window, text="This is the secondary window")
        label.grid(row=2, column=1, padx=10, pady=10)

        # Adding a close button centered
        scan_button = customtkinter.CTkButton(self.secondary_window, text="Scan for STM", command = lambda:sendData.find_stm32_port(self.init_textbox, label,set_parameters_button))
        scan_button.grid(row=3, column=1, padx=10, pady=10)


        set_parameters_button = customtkinter.CTkButton(self.secondary_window, text="Set Parameters", command =lambda:self.show_parameter_window(parameter=self.parameters, secondary=self.secondary_window))
        set_parameters_button.grid(row=4, column=1, padx=10, pady=30)
        set_parameters_button.configure(state=customtkinter.DISABLED)
        self.hide_main_window()




    def parameters(self):
        self.parameter_window = customtkinter.CTkToplevel(self)
        self.parameter_window.title("Parameter Window")
        self.parameter_window.geometry("700x500")
        self.parameter_window.transient(self)  # Set the parent of the secondary window
        self.parameter_window.grab_set()  # Redirect all events to this window

        # Configure grid weights
        self.parameter_window.grid_columnconfigure(0, weight=1)
        self.parameter_window.grid_columnconfigure(2, weight=1)
        self.parameter_window.grid_rowconfigure(0, weight=1)
        self.parameter_window.grid_rowconfigure(3, weight=1)

        # Load the image properly for PhotoImage in customtkinter
        # Note: PhotoImage should be imported from tkinter
        from tkinter import PhotoImage
        self.my_image = PhotoImage(file="ebara_logo.png")  # Correct file path necessary

        # Create a label to display the image in the secondary window
        self.image_label = customtkinter.CTkLabel(self.parameter_window, image=self.my_image, text="")
        self.image_label.grid(row=0, column=1, pady=10, sticky="n")

        
        self.string_input_button = customtkinter.CTkButton(self.parameter_window, text="Set Target Vac Pressure", command=lambda:self.open_input_dialog_event(text="Target Vac Pressure"))
        self.string_input_button.grid(row=1, column=0, pady=10)


        self.string_input_button = customtkinter.CTkButton(self.parameter_window, text="Set Target Back Pressure",command=lambda:self.open_input_dialog_event(text="Target Back Pressure"))
        self.string_input_button.grid(row=1, column=1, pady=10)

        self.string_input_button = customtkinter.CTkButton(self.parameter_window, text="Set Idle Time Limit",command=lambda:self.open_input_dialog_event(text="Idle Time Limit"))
        self.string_input_button.grid(row=1, column=2, pady=10)


        # Adding a label centered
        label = customtkinter.CTkLabel(self.parameter_window, text="This is the secondary window")
        label.grid(row=2, column=1, padx=10, pady=10)



        start_main_button = customtkinter.CTkButton(self.parameter_window, text="Open Program", command =lambda:self.show_main_window(secondary=self.parameter_window))
        start_main_button.grid(row=4, column=1, padx=10, pady=10)
        start_main_button.configure(state=customtkinter.NORMAL)
       

    def hide_main_window(self):
        # Hides the main window
        self.withdraw()

    def show_main_window(self,secondary):
        # Shows the main window again
        secondary.destroy()
        self.deiconify()

    def show_parameter_window(self,parameter,secondary):
        secondary.destroy()
        parameter()

    def hide_parameter_window(self,parameter):
        parameter.withdraw()

    def open_input_dialog_event(self, text):
        dialog = customtkinter.CTkInputDialog(text= text, title="CTkInputDialog")
        self.parameter_textbox = customtkinter.CTkTextbox(self.parameter_window, height=100, width=300, text_color="white", activate_scrollbars=False) 
        self.parameter_textbox.grid(row=2, column=1, pady=10, padx=10)


        input_result = dialog.get_input()  # This waits until the dialog is closed
        if input_result is not None:
            print("Input received:", input_result)
            stateData.db["parameters"][text] = input_result
            
            self.main_parameter_textbox.delete('1.0','end')
            for key in stateData.db["parameters"]:
                value = stateData.db["parameters"][key] 
                self.parameter_textbox.insert('end', f"{key} : {value}\n") 

                self.main_parameter_textbox.insert('end', f"{key} : {value}\n")  # Update the textbox

        print(stateData.db["parameters"])




    def show_content1(self):
        # Clear any existing content
        self.clear_main_content()

        self.testTitleLabel = customtkinter.CTkLabel(self.main_content, text="Welcome") 
        self.testTitleLabel.grid(row = 0, column=0, padx=10, sticky="nw")

        self.testTextLabel = customtkinter.CTkLabel(self.main_content, text="Current Test" ,
                                                    font=('',20),text_color="#6dd5ef")
        self.testTextLabel.grid(row = 0, column=1, pady=10,padx=10, sticky="nw")

        self.connectionLabel = customtkinter.CTkLabel(self.main_content, text="Hardware Connected" ,
                                                    font=('',20),text_color="#90EE90")
        self.connectionLabel.grid(row = 0, column=4, pady=10,padx=10, sticky="nw")


        # Add a button to the main content area
        self.pump1_button = customtkinter.CTkButton(self.main_content, height=150, width=150,text="Start \nPump 1" ,
                                                    command= lambda: self.handleSendData("STARTING",self.pump1_button))
        self.pump1_button.grid(row=2, column=3,  padx=10,pady=10, sticky="n")

        
        self.pump2_button = customtkinter.CTkButton(self.main_content,height=150, width=150, text="Start \n Pump 2")
        self.pump2_button.grid(row=2, column=4,  padx=10,pady=10, sticky="n")
        


        #DATA BOX

        
  # DATA BOX for title
        self.title_textbox = customtkinter.CTkTextbox(
            self.main_content, height=40, width=400, text_color="white", activate_scrollbars=False
        )
        # Place the title textbox at the top, spanning across the width of the window
        self.title_textbox.grid(row=1, column=1, padx=10, sticky="se")

        # DATA BOX for text
        self.textbox = customtkinter.CTkTextbox(
            self.main_content, height=400, width=400, text_color="white", activate_scrollbars=False
        )
        self.textbox.grid(row=2, column=1, pady = 10, padx=10, sticky="ne")

 
        self.main_parameter_textbox = customtkinter.CTkTextbox(self.main_content, height=100,text_color="#90EE90", width=200, activate_scrollbars=True)
        
        self.main_parameter_textbox.grid(row=2, column=3, pady=10, padx=10,sticky="s" )
       # self.main_parameter_textbox.tag_configure("greenText", foreground="#90EE90")

        


    def handleSendData(self,command,button):
        #delete contents in test status data
        '''
               with open(sendData.initDataFile,'w'):
            pass
        #start protocols and communtication with STM microcontroller
        '''
       
        #start protocols and communtication with STM microcontroller
              # Start a separate thread for serial communication
        self.serial_thread = threading.Thread(target=self.update_serial_data)
        self.serial_thread.daemon = True  # Daemonize the thread so it will be killed when the main thread exits
        self.serial_thread.start()
        self.pump1_button.configure(state="disabled", fg_color=self.start_button_color, text_color=self.disabled_text_color)
        self.pump2_button.configure(state="disabled", fg_color=self.disabled_button_color)

        #self.status_label = customtkinter.CTkLabel(self.main_content, text="PUMP 1 test is now active ", text_color="white")
        #self.status_label.grid(row=4, column=0, columnspan=2, pady=10)  # Adjust positioning as needed
       

        #self.start_serial_connection()
        
        self.startProgressBar()

       
        
    def update_serial_data(self):
        sendData.port = stateData.db['stmPORT']
        sendData.ser = serial.Serial(sendData.port, sendData.baud_rate , timeout=100)
        sendData.protocol_0(self.title_textbox,self.textbox, sendData.ser)
        try:
            while True:
                # Receive a single byte over serial
                received_byte = sendData.ser.read(1)
                
                if received_byte:
                    # Decode the byte and get the protocol function associated with it
                    sendData.protocol_func = sendData.protocols.get(ord(received_byte), None)

                    if sendData.protocol_func:
                        # Execute the protocol function
                        sendData.protocol_func(self.title_textbox, self.textbox, sendData.ser)
                        
                    else:
                        print("Unknown protocol for byte:", ord(received_byte))

        except KeyboardInterrupt:
            print("\nExiting program.")

        finally:
            # Close the serial port
            sendData.ser.close() 


    def update_textbox(self,text):
        self.textbox.insert('end',text)

    




    def show_content2(self):
        # Update to content 2
        self.clear_main_content()
        label = customtkinter.CTkLabel(self.main_content, text="Pump 1 Data")
        label.grid(pady=20, padx=20)

        label2 = customtkinter.CTkLabel(self.main_content,text="TEST")
        label2.grid(row = 1, column = 0)

        label3 = customtkinter.CTkLabel(self.main_content,text="TEST2")
        label3.grid(row = 1, column = 2)

 


    def startProgressBar(self):
        self.slider_progressbar_frame = customtkinter.CTkFrame(self.main_content, fg_color="transparent")
        self.slider_progressbar_frame.grid(row=4, column=1, padx=10 ,sticky="nsew")
        
        self.progressbar_1 = customtkinter.CTkProgressBar(self.slider_progressbar_frame, width=400,progress_color="green")
        self.progressbar_1.grid(row=4, column=1, sticky="e")
        self.progressbar_1.configure(mode="indeterminate")
        self.progressbar_1.start()
       

    def show_content3(self):
        # Update to content 3z
        self.clear_main_content()
        label = customtkinter.CTkLabel(self.main_content, text="Pump 2 Data")
        label.grid(pady=20, padx=20)
        
        textbox = customtkinter.CTkTextbox(self.main_content,height=200 ,width=400)
        textbox.grid(row=2, column=2)

        textbox.insert("0.0", "CTkTextbox\n\n" +"Pump 2 Data Here\n\n")

    def show_content4(self):
        # Update to content 4
        self.clear_main_content()
        label = customtkinter.CTkLabel(self.main_content, text="Pump Status Data")
        label.grid(pady=20, padx=20)
        
        self.textbox = customtkinter.CTkTextbox(self.main_content, height=500, width=500)
        self.textbox.grid(row=2, column=2)

        self.update_serial_data()

    def start_serial_connection(self):
        self.ser = serial.Serial('COM13', 115200, timeout=1)
        self.ser.flushInput()  # Clear any existing data in the buffer


  






if __name__ == '__main__':
    #webapp.run(debug=True, port = 8080)

    app = App()
    app.mainloop()
 
'''
if __name__ == "__main__":
    app = App()
    app.mainloop()
'''