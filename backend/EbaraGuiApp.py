import customtkinter
import serial
import time
import sendData
import saveFile
#from sendData import *



#customtkinter.set_appearance_mode("System")  # Modes: "System" (standard), "Dark", "Light"
#customtkinter.set_default_color_theme("blue")

class App(customtkinter.CTk):
    WIDTH = 1000
    HEIGHT = 600

    def __init__(self):
        super().__init__()

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

        # Sidebar buttons
        self.sidebar_button_1 = customtkinter.CTkButton(self.sidebar_frame, text="Pumps", command=self.show_content1)
        self.sidebar_button_1.grid(row=1, column=0, pady=10, padx=20)

        self.sidebar_button_2 = customtkinter.CTkButton(self.sidebar_frame, text="Pump 1 Data", command=self.show_content2)
        self.sidebar_button_2.grid(row=2, column=0, pady=10, padx=20)

        self.sidebar_button_3 = customtkinter.CTkButton(self.sidebar_frame, text="Pump 2 Data", command=self.show_content3)
        self.sidebar_button_3.grid(row=3, column=0, pady=10, padx=20)

        self.sidebar_button_4 = customtkinter.CTkButton(self.sidebar_frame, text="Pump Status Data", command=self.show_content4)
        self.sidebar_button_4.grid(row=4, column=0, pady=10, padx=20)

        self.sidebar_button_5 = customtkinter.CTkButton(self.sidebar_frame, text="Test Data", command=saveFile.openFileExplorer)
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

    def show_content1(self):
        # Clear any existing content
        self.clear_main_content()
        
        label = customtkinter.CTkLabel(self.main_content, text="Choose Pump Model")
        label.grid(row=0, column=0, pady=20, padx=100)
        #Option drop down for pump model
        self.optionmenu_1 = customtkinter.CTkOptionMenu(master=self.main_content, values=["Pump Model 1", "Pump Model 2", "Pump Model 3", "Pump Model 4"])
        self.optionmenu_1.grid(row=1, column=0, padx=20)

        # Add a label to the main content area
        label = customtkinter.CTkLabel(self.main_content, text="Choose a pump to start testing")
        label.grid(row=2, column=0, pady=20, padx=20)
        


        # Add a button to the main content area
        self.pump1_button = customtkinter.CTkButton(self.main_content, height=200, width=200,text="Start \nPump 1" ,
                                                    command= lambda: self.handleSendData("STARTING",self.pump1_button))
        self.pump1_button.grid(row=3, column=0, pady=5, padx=20)  # Using grid for consistency

        
        self.pump2_button = customtkinter.CTkButton(self.main_content,height=200, width=200, text="Start \n Pump 2")
        self.pump2_button.grid(row=4, column=0, pady=5, padx=20)  # Using grid for consistency
        

        #DATA BOX

        
        self.textbox = customtkinter.CTkTextbox(self.main_content, height=400, width=400,text_color="white", activate_scrollbars=False)
        # Place the textbox right next to the label, ensuring it's in a column that aligns it to the right
        self.textbox.grid(row=3, column=4, rowspan=4, padx=10)  # rowspan adjusts how many rows it spans; "ne" aligns it to the top-right

        # Ensure main_content is configured to push widgets to the right
        self.main_content.grid_columnconfigure(4, weight=1)  # Make the second last column expand, pushing column 5 to the right

    def handleSendData(self,command,button):
        #delete contents in test status data
        with open(sendData.initDataFile,'w'):
            pass
        #start protocols and communtication with STM microcontroller
        sendData.protocol_0()
        self.pump1_button.configure(state="disabled", fg_color=self.start_button_color, text_color=self.disabled_text_color)
        self.pump2_button.configure(state="disabled", fg_color=self.disabled_button_color)

        self.status_label = customtkinter.CTkLabel(self.main_content, text="PUMP 1 test is now active ", text_color="white")
        self.status_label.grid(row=4, column=0, columnspan=2, pady=10)  # Adjust positioning as needed


        #self.start_serial_connection()
        self.update_serial_data()
        self.startProgressBar()




    def update_serial_data(self):
        reader = open(sendData.initDataFile)
        '''
       if sendData.textbox_data:
            self.textbox.insert('end', sendData.textbox_data + '\n')  # Append data to the textbox
        '''
        
        initData = reader.read()
        reader.close()
        self.textbox.delete("0.0",'end')
        self.textbox.insert("0.0",initData)
        
        self.after(100, self.update_serial_data)  # Schedule next check

    def update_textbox(self,text):
        self.textbox.insert('end',text)

    




    def show_content2(self):
        # Update to content 2
        self.clear_main_content()
        label = customtkinter.CTkLabel(self.main_content, text="Pump 1 Data")
        label.grid(pady=20, padx=20)

        textbox = customtkinter.CTkTextbox(self.main_content,height=200 ,width=400)
        textbox.grid(row=2, column=2)

        textbox.insert("0.0", "CTkTextbox\n\n" +"Pump 1 Data Here\n\n")

    def startProgressBar(self):
        self.slider_progressbar_frame = customtkinter.CTkFrame(self.main_content, fg_color="transparent")
        self.slider_progressbar_frame.grid(row=2, column=4, padx=(0, 0), pady=(0, 0), sticky="nsew")
        self.slider_progressbar_frame.grid_columnconfigure(0, weight=1)  # Ensure the column expands fully
        self.slider_progressbar_frame.grid_rowconfigure(1, weight=1)  # Ensure the row of the progress bar can expand as needed
        self.progressbar_1 = customtkinter.CTkProgressBar(self.slider_progressbar_frame, width=400,progress_color="green")
        self.progressbar_1.grid(row=0, column=0, sticky="")
        self.progressbar_1.configure(mode="indeterminate")
        self.progressbar_1.start()
       

    def show_content3(self):
        # Update to content 3
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