import serial as ser
import time
import math
import turtle
import keyboard
from gui import *

class PythonComm():
    def __init__(self):
        self.uart = ser.Serial('COM3', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)   # timeout of 1 sec where the read and write operations are blocking,
                                # after the timeout the program continues

    def resetIO(self):
        self.uart.reset_input_buffer()
        self.uart.reset_output_buffer()

    def transmitStateNum(self, state):
        self.resetIO()
        stateChar = bytes(state, 'ascii') 
        self.uart.write(stateChar)
        time.sleep(0.25)
        
    def state2(self, tur):
        mouseDirX = 0
        mouseDirY = 0
        currentX = 0
        currentY = 0
        angle = ''

        tur.penup()

        while True:
            if keyboard.is_pressed('q'):
                    break
                    
            try: 
                while (self.uart.in_waiting > 0): # While buffer is not empty
                    lineByte = self.uart.read(size = 1) # Read from buffer 
                    lineByte = lineByte.decode("utf-8")

                    if(lineByte == 'n'):
                        tur.penup()  

                    elif(lineByte == 'w'):
                        tur.pendown()
                        tur.pen(pencolor="black")

                    elif(lineByte == 'c'):
                        tur.pendown()
                        tur.pen(pencolor="White")                   
                        
                    if(lineByte != '\n' and len(angle) < 4 and ('0' <= lineByte <= '9')):
                        angle = angle + lineByte

                    else:
                        angleInt = int(angle[0])*100 + int(angle[1])*10 + int(angle[2])

                        if (angleInt >= 0 and angleInt <= 360):
                            rad = (angleInt /360) * (2 * math.pi)

                            mouseDirX = (1 * math.sin(rad))
                            mouseDirY = (1 * math.cos(rad))

                            currentX += mouseDirX
                            currentY += mouseDirY

                        tur.goto(currentX,currentY)

                        yield(angle)
                        angle = ''
            except:
                pass

    def state3Proceed(self):
        bytesChar = bytes('1', 'ascii') 
        bytesChar = bytesChar # + bytes('\n', 'ascii')
        self.uart.write(bytesChar)
        time.sleep(0.25)

    def state3(self):
        while (self.uart.in_waiting > 0):  # while the input buffer isn't empty 
            lineByte = self.uart.read_until('\n')    # read  from the buffer until the terminator is received
            lineStr=""
            for i, item  in enumerate(lineByte):
                newChar = (item.to_bytes(1,'big')).decode("utf-8")
                lineStr = lineStr + newChar

        steps = int(lineStr)
        angle = round(360 / steps, 3)
     
        time.sleep(0.25)  # delay for accurate read/write operations on both ends

        return steps, angle


    def state4(self, path):
        # Extract the file name from the 
        fileName = path.rsplit('/', 1)[-1]
        fileName = fileName.split('.', 1)[0]
        fileName += 'Z'

        # Read the script's txt file to a string:
        with open(path) as f:
            scriptStr = f.read()
        scriptStr += 'ZZ'

        # Send script name to the microcontroller:
        for item in fileName:   
            b = item.encode('utf-8')
            print(b)
            self.uart.write(b)
            time.sleep(0.01)

        # Send script size (in bytes) to the microcontroller:
        fileSize = str(len(scriptStr)) + 'Z' 

        for item in fileSize:   
            b = item.encode('utf-8')
            print(b)
            self.uart.write(b)
            time.sleep(0.01)

        # Send the script's content to the microcontorller:
        for item in scriptStr:   
            b = item.encode('utf-8')
            print(b)
            self.uart.write(b)
            time.sleep(0.01)   

        acks = 0 # Acknowledgments counter
        angle = ''

        while (acks < 2): # Finish after received 2 acks
            while (self.uart.in_waiting > 0 and acks < 2):

                lineByte = self.uart.read(size = 1) # Read from buffer, until 'Z' is received
                lineByte = lineByte.decode("utf-8")

                if(lineByte == 'a'):
                    acks += 1
                    if(acks == 2):   
                        break

                if(lineByte == 'Z'): 
                    break

                elif(lineByte != '\n'): # Receive the current angle form the microcontroller
                    angle = angle + lineByte

                else:
                    yield(angle)
                    print(angle)
                    angle = ''

                if(lineByte == 'Z'): 
                    break
            
            if(acks == 2): 
                break
            
        