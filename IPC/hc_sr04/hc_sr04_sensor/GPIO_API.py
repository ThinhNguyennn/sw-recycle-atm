import Jetson.GPIO as GPIO

#function of set mode:
def DIO_SetModePin(pin_number, modepin):
    if modepin == "input":
        GPIO.setup(pin_number, GPIO.IN)
    elif modepin == "output":
        GPIO.setup(pin_number,  GPIO.OUT)
    else:
        print("Invalid mode. Please choose either 'input' or 'output'.")

#function of set up:
def DIO_SetUp(mode):
    if mode == "BCM":
        GPIO.setmode(GPIO.BCM)          # GPIO7 : GPIO7 (BCM)
    elif mode == "BOARD":
        GPIO.setmode(GPIO.BOARD)        #GPIO7: PIN7 
    else:
        print("Invalid mode. Please choose either 'BCM' or 'BOARD'.")

#function of output mode:
def DIO_WritePin(pin_number, state):
    if state == "HIGH":
        GPIO.output(pin_number, GPIO.HIGH)
    if state == "LOW":
        GPIO.output(pin_number, GPIO.LOW) 

#function of input mode:
def DIO_ReadPin(pin_number):
    state = GPIO.input(pin_number)
    return state

def DIO_cleanup():
 GPIO.cleanup()


