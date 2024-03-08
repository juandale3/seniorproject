import threading
import sendData
import EbaraGuiApp
import time
# Assuming file1.py and file2.py have main functions named file1_main() and file2_main()

thread1 = threading.Thread(target=EbaraGuiApp.app)
thread2 = threading.Thread(target=sendData.listen)

thread1.start()
thread2.start()

thread1.join()
thread2.join()
