#!/usr/bin/python3

# Steering Wheel Controls
# Read inputs from Arduino via serial over USB

#import RPi.GPIO as GPIO  # Add later with more features
from subprocess import call
import shlex
import time
import serial
import Pyro4

# GPIO.setmode(GPIO.BOARD) # Add later with more features
currentVolume = 50
modePressed = 0
modeStart = 0
modeFinish = 0
pressedTime = 2
lastPressed = 0

# Enable anything to be able to read the arduino usb
call(shlex.split('sudo chmod 777 /dev/ttyUSB0'))
time.sleep(3.0)
ardPort = serial.Serial(port='/dev/ttyUSB0', baudrate=9600) # Which usb port is connected to the arduino
time.sleep(1.0)

while True: #changed from if ardPort.isOpen to while True
    try:
        readArd = ardPort.readline().strip()
        ardDecode = readArd.decode("UTF-8")
    
        # Wire 1: Short: Mode,  165 ohms: Volume Up,   652 ohms: Seek Up
        # Wire 2: Short: Power, 165 ohms: Volume Down, 652 ohms: Seek Down
        if int(modeFinish) > int(time.time()):
            modePressed = 0
    
            # Ok, I wanted a function button so Mode will be it, it will auto
            # turn off after 1 minute or if pressed again within that time.
            # This will allow other buttons to have improved functionality
        if '101' in ardDecode: # Button -- Mode 
        # This checks how long the button
            if modePressed == 0:
                modePressed = 1
                modeStart = time.time()
                modeFinish = modeStart + 60
                call(shlex.split('xbmc-send --action="ActivateWindow(favourites)"'))
                call(shlex.split('xbmc-send --action="Action(select)"'))
            if modePressed == 1:
                modePressed = 0
                call(shlex.split('xbmc-send --action="ActivateWindow(home)"'))
    
        if '102' in ardDecode: # Button -- Volume Up
            if modePressed == 0:
                call(shlex.split('xbmc-send --action="Action(up)"'))
            else:
                videoControl = Pyro4.Proxy("PYRONAME:control.volume")
                print(videoControl.volume_control(5))
    
        if '103' in ardDecode: # Button -- Seek Up
            if modePressed == 0:
                call(shlex.split('xbmc-send --action="PlayerControl(Next)"'))
            else:
                call(shlex.split('xbmc-send --action="Action(ParentDir)"'))
    
        if '201' in ardDecode: # Button -- Power
            if modePressed == 0:
                call(shlex.split('xbmc-send --action="PlayerControl(Play)"'))
            else:
                call(shlex.split('xbmc-send --action="PlayerControl(PartyMode)"'))
                sleep(0.2)
                call(shlex.split('xbmc-send --action="Action(select)"'))
                
        if '202' in ardDecode: # Button -- Volume Down
            if modePressed == 0:
                call(shlex.split('xbmc-send --action="Action(down)"'))
            else:
                videoControl = Pyro4.Proxy("PYRONAME:control.volume")
                print(videoControl.volume_control(-5))
    
        if '203' in ardDecode: # Button -- Seek Down
            if modePressed == 0:
                call(shlex.split('xbmc-send --action="PlayerControl(Previous)"'))
            else:
                call(shlex.split('xbmc-send --action="Action(select)"'))
    
        if '111' in ardDecode: # The car is off, night night
            suspendSystem = Pyro4.Proxy("PYRONAME:system.suspend")
            print(suspendSystem.suspend_system_now(0))
    
        if '222' in ardDecode: # The car is on, Wakey Wakey
            resumeSystem = Pyro4.Proxy("PYRONAME:system.resume")
            print(resumeSystem.resume_system_now(0))
    
        ardPort.flushInput() #I may or may not need this part
    
    except OSError as err:
        print("OS error: {0}".format(err))
