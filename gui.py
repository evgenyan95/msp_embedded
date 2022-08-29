from tkinter import *
from tkinter import filedialog
from pythonComm import *
import sys
from turtle import *

class Gui():
    def __init__(self, root):
        self.pythonComm = PythonComm()

        self.root = root
        self.root.resizable = (False, False)
        self.root.title("DCS Project - Jenia & Dor")
        self.state = 0

        # Labels:
        self.mainMenuTitle1 = Label(self.root, text = "DCS 2022 Final Project", pady = 5, font = ("Heebo", 24, 'bold'))
        self.mainMenuTitle2 = Label(self.root, text = "Main Menu", pady= 20, font = ("Heebo", 18, 'bold underline'))

        ##### State 1 labels:
        self.waitLabel = Label(self.root, text = "Please wait for the dial to return to 0 degrees.",
                          pady= 10, font = ("Heebo", 16))
        self.moveStickLabel = Label(self.root, text = "You can now control the dial's angle using the joystick.",
                          pady= 10, font = ("Heebo", 16))

        ##### State 2 labels:
        self.opt2AngleLabel = Label(self.root, text = "Current movement angle: 0", pady= 10, font = ("Heebo", 16))

        ##### State 3 labels:
        self.startRotateLabel = Label(self.root, text = "Please click on 'Start Rotating' to start the motor's rotation.",
                          pady= 10, font = ("Heebo", 16))
        self.set0DegLabel = Label(self.root, text = "Please click on 'Set 0 degrees' when the dial reaches 0 degrees.",
                          pady= 10, font = ("Heebo", 16))
        self.stopLabel = Label(self.root, text = "Please click on 'Stop' after the dial reaches back to 0 degrees.",
                          pady= 10, font = ("Heebo", 16))
        self.stepsLabel = Label(self.root, text = 'No. of steps: 0', pady= 10, font = ("Heebo", 16, 'bold'))
        self.opt3AngleLabel = Label(self.root, text = 'Step angle: 0', pady= 10, font = ("Heebo", 16 ,'bold'))

        # State 4 labels:
        self.runningScriptLabel = Label(self.root, text = 'Running Script...', pady= 10, font = ("Heebo", 16 ,'bold'))
        self.opt4AngleLabel = Label(self.root, text = 'Current dial angle: 0', pady= 10, font = ("Heebo", 16 ,'bold'))

        # Buttons:
        self.option1Button = Button(self.root, text = "1) Manual control of motor based machine", width = 40, font = ("Heebo", 16), command = self.opt1)
        self.option2Button = Button(self.root, text = "2) Joystick based PC painter", width = 40, font = ("Heebo", 16), command = self.opt2)
        self.option3Button = Button(self.root, text = "3) Stepper motor calibration", width = 40, font = ("Heebo", 16), command = self.opt3)
        self.option4Button = Button(self.root, text = "4) Script mode", width = 40, font = ("Heebo", 16), command = self.opt4)
        self.option5Button = Button(self.root, text = "5) Exit", width = 40, font = ("Heebo", 16), command = self.exit)
        self.backButton = Button(self.root, text = "Back", font = ("Heebo", 16), command = self.back)

        ##### State 3 buttons:
        self.startRotateButton = Button(self.root, text = "Start Rotating", font = ("Heebo", 16), command = self.startRotate)
        self.set0DegButton = Button(self.root, text = "Set 0 Degerees", font = ("Heebo", 16), command = self.set0Deg)
        self.stopButton = Button(self.root, text = "Stop", font = ("Heebo", 16), command = self.stop)

        ##### State 4 buttons:
        self.selectScript1Button = Button(self.root, text = "Select script #1", font = ("Heebo", 16), command = self.selectScript)
        self.selectScript2Button = Button(self.root, text = "Select script #2", font = ("Heebo", 16), command = self.selectScript)
        self.selectScript3Button = Button(self.root, text = "Select script #3", font = ("Heebo", 16), command = self.selectScript)

        # Credits:
        self.credits = Label(root, text = "By Jenia & Dor", pady = 20, font = ("Heebo", 12))

        self.mainMenu()

    def mainMenu(self):
        self.mainMenuTitle1.grid(row = 0, column = 0)
        self.mainMenuTitle2.grid(row = 1, column = 0)
        self.option1Button.grid(row = 2, column = 0)
        self.option2Button.grid(row = 3, column = 0)
        self.option3Button.grid(row = 4, column = 0)
        self.option4Button.grid(row = 5, column = 0)
        self.option5Button.grid(row=6, column = 0)
        self.credits.grid(row = 7, column = 0)

        self.stopButton["state"] = "normal"

        self.state = 0

    def back(self):
        self.pythonComm.resetIO()
        self.clearWindow()
        self.mainMenu()

    def exit(self):
        self.root.destroy()
        sys.exit()

    def activateAllButtons(self):
        for widget in self.root.winfo_children():
            widget.grid_forget()

    def clearWindow(self):
        for widget in self.root.winfo_children():
            widget.grid_forget()

    def opt1(self):
        self.clearWindow()  
        self.waitLabel.grid(row = 0, column = 0)
        self.backButton.grid(row = 1 ,column = 0)
        
        self.pythonComm.transmitStateNum('3')
        self.pythonComm.resetIO()
        self.pythonComm.transmitStateNum('2')

        self.waitLabel.grid_forget()
        self.moveStickLabel.grid(row = 0 ,column = 0)

    def opt2(self):
        self.clearWindow()

        canvas = Canvas(self.root, width = 500, height = 500)
        tur = RawTurtle(canvas)

        canvas.grid(row = 0, column = 0)
        self.opt2AngleLabel.grid(row = 1, column = 0)
        self.backButton.grid(row = 2, column = 0)

        self.pythonComm.transmitStateNum('5')
        
        for angle in self.pythonComm.state2(tur):
            self.opt2AngleLabel["text"] = f"Current movement angle: {angle}"
    
    def opt3(self):
        self.clearWindow()
        self.startRotateLabel.grid(row = 0, column = 1)
        self.stepsLabel.grid(row = 1, column = 0)
        self.opt3AngleLabel.grid(row = 1, column = 2)
        self.startRotateButton.grid(row = 2, column = 1)
        self.backButton.grid(row = 3, column = 1)

    def opt4(self):
        self.pythonComm.transmitStateNum('4')
        
        self.clearWindow()
        self.selectScript1Button.grid(row = 0, column = 0)
        self.selectScript2Button.grid(row = 0, column = 1)
        self.selectScript3Button.grid(row = 0, column = 2)
        self.opt4AngleLabel.grid(row = 2, column = 1)
        self.backButton.grid(row = 3, column = 1)

    def startRotate(self):
        self.pythonComm.transmitStateNum('1')

        self.startRotateLabel.grid_forget()
        self.set0DegLabel.grid(row = 0, column = 1)
        self.startRotateButton.grid_forget()
        self.set0DegButton.grid(row = 2, column = 1)

    def set0Deg(self):
        self.pythonComm.state3Proceed()

        self.set0DegLabel.grid_forget()
        self.stopLabel.grid(row = 0, column = 1)
        self.set0DegButton.grid_forget()
        self.stopButton.grid(row = 2, column = 1)

    def stop(self):
        self.stopButton["state"] = "disabled"

        self.pythonComm.state3Proceed()

        steps, angle = self.pythonComm.state3()

        self.stepsLabel["text"]  = f'No. of steps: {steps}'
        self.opt3AngleLabel["text"] = f'Step angle: {angle}'

    def selectScript(self):
        self.selectScript1Button["state"] = "disabled"
        self.selectScript2Button["state"] = "disabled"
        self.selectScript3Button["state"] = "disabled"
        self.backButton["state"] = "disabled"

        fileTypes = (('text files', '*.txt'), )

        path = filedialog.askopenfilename(
        title = 'Script Select',
        initialdir = '/',
        filetypes = fileTypes)

        self.runningScriptLabel.grid(row = 1, column = 1)
        
        for angle in self.pythonComm.state4(path):
            self.opt4AngleLabel["text"] = f'Current dial angle: {angle}'

        self.runningScriptLabel.grid_forget()

        self.selectScript1Button["state"] = "normal"
        self.selectScript2Button["state"] = "normal"
        self.selectScript3Button["state"] = "normal"
        self.backButton["state"] = "normal"
        
       




