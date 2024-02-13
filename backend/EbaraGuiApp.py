import customtkinter
import serial
import time

from flask import Flask
webapp = Flask(__name__)

# Setup serial connection
ser = serial.Serial('COM9', 115200, timeout=1)

# Flush any remaining input buffer to start fresh
ser.flushInput()

#customtkinter.set_appearance_mode("System")  # Modes: "System" (standard), "Dark", "Light"
#customtkinter.set_default_color_theme("blue")

class App(customtkinter.CTk):
    WIDTH = 1000
    HEIGHT = 600

    def __init__(self):
        super().__init__()

        self.title("Semi-Automated Pump Testing Station")
        self.geometry(f"{App.WIDTH}x{App.HEIGHT}")

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
        
        self.sidebar_button_5 = customtkinter.CTkButton(self.sidebar_frame, text="Send Serial Data", command=self.show_content4)
        self.sidebar_button_5.grid(row=5, column=0, pady=10, padx=20)

        # Main content area
        # Lighter shade for the main content area
        self.main_content = customtkinter.CTkFrame(self, fg_color="gray15")
        self.main_content.grid(row=0, column=1, sticky="nsew")
        self.grid_columnconfigure(1, weight=1)
        self.grid_rowconfigure(0, weight=1)

        
    def clear_main_content(self):
        # Clear the main content area
        for widget in self.main_content.winfo_children():
            widget.destroy()

    def show_content1(self):
        # Clear any existing content
        self.clear_main_content()

        # Add a label to the main content area
        label = customtkinter.CTkLabel(self.main_content, text="Choose a pump to start testing")
        label.grid(row=0, column=0, pady=20, padx=20)

        # Add a button to the main content area
        self.pump1_button = customtkinter.CTkButton(self.main_content, height=200, width=200,text="Start \nPump 1")
        self.pump1_button.grid(row=1, column=0, pady=20, padx=20)  # Using grid for consistency

        
        self.pump2_button = customtkinter.CTkButton(self.main_content,height=200, width=200, text="Start \n Pump 2")
        self.pump2_button.grid(row=1, column=2, pady=20, padx=20)  # Using grid for consistency


    def show_content2(self):
        # Update to content 2
        self.clear_main_content()
        label = customtkinter.CTkLabel(self.main_content, text="Pump 1 Data")
        label.grid(pady=20, padx=20)

        textbox = customtkinter.CTkTextbox(self.main_content,height=200 ,width=400)
        textbox.grid(row=2, column=2)

        textbox.insert("0.0", "CTkTextbox\n\n" +"Pump 1 Data Here\n\n")

        
       

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

    def update_serial_data(self):
        if ser.in_waiting > 0:
            serialData = ser.readline().decode('utf-8')  # Decode data to string
            print(serialData)  # This will print the decoded string
            self.textbox.insert('end', serialData + '\n')  # Insert data at the end of the textbox
        self.after(100, self.update_serial_data)  # Schedule this method to be called again after 100ms

            
@webapp.route('/')
def root():
    return 'Root Page loaded'







if __name__ == '__main__':
    #webapp.run(debug=True, port = 8080)
    app = App()
    app.mainloop()

'''
if __name__ == "__main__":
    app = App()
    app.mainloop()
'''