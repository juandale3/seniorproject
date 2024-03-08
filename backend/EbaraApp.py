import threading

import sendData
from EbaraGuiApp import App


#threading, running EbaraGuiApp and sendData.py
def runGUI():
    app = App()
    app.mainloop()

def runSendData():
    # Starting the Flask web app
    sendData.listen()
if __name__ == "__main__":
    # Create threads for Flask and customtkinter apps
    sendData_thread = threading.Thread(target=runSendData)
    gui_thread = threading.Thread(target=runGUI)

    # Start the threads
    sendData_thread.start()
    gui_thread.start()

    # Join threads to the main thread
    sendData_thread.join()
    gui_thread.join()
