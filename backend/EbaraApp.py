import threading
import customtkinter

from EbaraGuiApp import App
from EbaraWebApp import webapp


def runGUI():
    app = App()
    app.mainloop()

def runWebApp():
    # Starting the Flask web app
    webapp.run(debug=True, port=8080, use_reloader=False)

if __name__ == "__main__":
    # Create threads for Flask and customtkinter apps
    flask_thread = threading.Thread(target=runWebApp)
    gui_thread = threading.Thread(target=runGUI)

    # Start the threads
    flask_thread.start()
    gui_thread.start()

    # Join threads to the main thread
    flask_thread.join()
    gui_thread.join()
