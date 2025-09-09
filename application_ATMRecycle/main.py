import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton
from ui_main import Ui_MainWindow
from PyQt5.QtCore import QThread, pyqtSignal
import sysv_ipc
from multiprocessing import Value, Lock

##################################################################################################### 
#                                                                                                   #
#                                DEFINE THE MACRO AND VARIABLE                                      #
#                                                                                                   #
#####################################################################################################
send_key = 12345                # key 12345 is key id for btn message between FSM and Application 
receive_key = 56789             # key 56789 is key id for page message between FSM and Application 
message_queue = sysv_ipc.MessageQueue(send_key, sysv_ipc.IPC_CREAT)     # creating the Queue of Sending Key

Page_message = ''                 # Set Page_message = NULL when first creating variable
message_flag = Value('b', False)  # Using multiprocessing.Value for all process
message_lock = Lock()             # Lock for safable


##################################################################################################### 
#                                                                                                   #
#                                DEFINE THE FUNCTION AND GUI                                        #
#                                                                                                   #
#####################################################################################################

#####################################################################################################
#   Class name: ReceiverThread(QThread)
#   Function: Creating thread to receive messages form any Queue
#   Return: Signal for GUI of PyQt5
#####################################################################################################
class ReceiverThread(QThread):
    message_received = pyqtSignal(str)      # Creating Signal to GUI from ReceiverThread

    def __init__(self, receive_key, flag, lock, parent=None):
        super().__init__(parent)
        self.message_queue = sysv_ipc.MessageQueue(receive_key, sysv_ipc.IPC_CREAT) #Creating receive page Queue from FSM
        self.flag = flag        # Set flag
        self.lock = lock        # Set Lock

    # Creating While loop to wait the data 'page' from FSM
    def run(self):
        while True:
            try:
                # Receive message from FSM
                message, _ = self.message_queue.receive()
                # Change byte to String data
                decoded_message = message.decode('utf-8')
                # Send the message string to GUI
                self.message_received.emit(decoded_message)
                # Set flag on when having message
                with self.lock:
                    self.flag.value = True
            except sysv_ipc.ExistentialError:           # Print error when Queue does not exist
                print(f"Message Queue with key {self.message_queue.key} does not exist.")

#####################################################################################################
#   Class name: MainWindow(Ui_MainWindow)
#   Function: Creating the interface inherit from ui_main.py and define the function for the interface
#   Return: GUI of application user
#####################################################################################################
class MainWindow(Ui_MainWindow):

    def __init__(self, parent=None):
        super().__init__()
        self.setupUi(parent)

        # Link the btn to string name of page
        self.pages = {
            self.InformationBtn: "Information",
            self.ContactBtn: "Contact",
            self.GarbageBtn: "Garbage",
            self.PlusPointBtn: "PlusPoint"
        }

        # Creating and run thread for receiver      *
        self.new_receiver_thread = ReceiverThread(56789, message_flag, message_lock)
        self.new_receiver_thread.message_received.connect(self.on_message_received)
        self.new_receiver_thread.start()

        # For loop to catch the click btn and call function on_button_clicked()
        for button in self.pages:
            button.clicked.connect(lambda _, b=button: self.on_button_clicked(b))

#####################################################################################################
#   Function name: reset_button_colors(self)
#   Function: Reset the animation of btn 
#   Return: None
#####################################################################################################
    def reset_button_colors(self):
        for btn in self.pages.keys():
            btn.setStyleSheet("")

#####################################################################################################
#   Function name: on_button_clicked(self, button)
#   Function: the function of btn when clicking
#   Return: None
#####################################################################################################
    def on_button_clicked(self, button):
        message_queue.send(self.pages[button])  # Send name page of button clicking to FSM
        print(f"Button clicked. Corresponding page: {self.pages[button]}")
        self.reset_button_colors()  #Reset btn colors
        button.setStyleSheet("background-color: #00A9FF;") #Set color for btn choosed

#####################################################################################################
#   Function name: closeEvent(self, event)
#   Function: Event which is called when the application closes
#   Return: None
#####################################################################################################
    def closeEvent(self, event):
        # remove queue send to FSM
        message_queue.remove()
        print(f"Remove Queue....")
        event.accept()

#####################################################################################################
#   Function name: on_message_received(self, message)
#   Function: Changing page when message page FSM receive to Application
#   Return: None
#####################################################################################################
    def on_message_received(self, message):
        cleaned_message = message.rstrip('\x00')    #clear the value '\x00' in the end of message string receive
        print(f"Received message from new key: {message}")
        
        # processing value of message page and reset Flag
        with message_lock:
            global Page_message
            Page_message = cleaned_message  # get the page to the variable Page_message
            message_flag.value = False      # Reset flag
            print("Page_message:", Page_message)

            #processing changes page
            if Page_message == "Garbage":
                self.Page_Widget.setCurrentWidget(self.Page_Garbage)
            elif Page_message == "Information":
                self.Page_Widget.setCurrentWidget(self.Page_Information)
            elif Page_message == "Contact":
                self.Page_Widget.setCurrentWidget(self.Page_Contact)
            else:
                self.Page_Widget.setCurrentWidget(self.Page_PlusPoint)


#---------------------------------------- MAIN FUNCTION -------------------------------------------#

if __name__ == "__main__":
    app = QApplication(sys.argv)
    main_window = QMainWindow()
    ui = MainWindow(main_window)
    main_window.show()

    # Kết nối sự kiện đóng cửa sổ với hàm closeEvent
    main_window.closeEvent = ui.closeEvent

    sys.exit(app.exec_())
