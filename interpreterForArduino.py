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
modeFinish = 0

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
            ## Or, I just made short and long press functionality, now I have more buttons
            ## than I know what to do with at this point.
        if '101' in ardDecode: # Button -- Mode 
            if modePressed == 0:
                modePressed = 1
                modeFinish = time.time() + 60
                call(shlex.split('xbmc-send --action="ActivateWindow(fileManager,sources://music/)"'))
            if modePressed == 1:
                modePressed = 0
                call(shlex.split('xbmc-send --action="ActivateWindow(home)"'))
        
        if '111' in ardDecode: # Button -- ModeLong
            if modePressed == 0:
                modePressed = 1
                modeFinish = time.time() + 60
                call(shlex.split('xbmc-send --action="ActivateWindow(fileManager,sources://music/)"'))
            if modePressed == 1:
                modePressed = 0
                call(shlex.split('xbmc-send --action="ActivateWindow(home)"'))

        if '102' in ardDecode: # Button -- Volume Up
            if modePressed == 1:
                call(shlex.split('xbmc-send --action="Action(Up)"'))
            else:
                volumeControl = Pyro4.Proxy("PYRONAME:control.volume")
                print(volumeControl.volume_control(5))

        if '112' in ardDecode: # Button -- Volume Up Long
            if modePressed == 1:
                call(shlex.split('xbmc-send --action="Action(PageUp)"'))
            else:
                volumeControl = Pyro4.Proxy("PYRONAME:control.volume")
                print(volumeControl.volume_control(5))
    
        if '103' in ardDecode: # Button -- Seek Up
            if modePressed == 1:
                call(shlex.split('xbmc-send --action="Action(Left)"'))
            else:
                call(shlex.split('xbmc-send --action="PlayerControl(Next)"'))
    
        if '113' in ardDecode: # Button -- Seek Up Long
            if modePressed == 1:
                call(shlex.split('xbmc-send --action="Action(Left)"'))
            else:
                call(shlex.split('xbmc-send --action="PlayerControl(Seek(15))"'))


        if '201' in ardDecode: # Button -- Power
            if modePressed == 1:
                call(shlex.split('xbmc-send --action="PlayerControl(PartyMode)"'))
                sleep(0.2)
                call(shlex.split('xbmc-send --action="Action(select)"'))
            else:
                call(shlex.split('xbmc-send --action="PlayerControl(Play)"'))

        if '211' in ardDecode: # Button -- Power Long
            if modePressed == 1:
                call(shlex.split('xbmc-send --action="Action(Select)"'))
            else:
                call(shlex.split('xbmc-send --action="PlayerControl(Play)"'))

        if '202' in ardDecode: # Button -- Volume Down
            if modePressed == 1:
                call(shlex.split('xbmc-send --action="Action(Down)"'))
            else:
                volumeControl = Pyro4.Proxy("PYRONAME:control.volume")
                print(volumeControl.volume_control(-5))
        
        if '212' in ardDecode: # Button -- Volume Down Long
            if modePressed == 1:
                call(shlex.split('xbmc-send --action="Action(PageDown)"'))
            else:
                volumeControl = Pyro4.Proxy("PYRONAME:control.volume")
                print(volumeControl.volume_control(-5))

        if '203' in ardDecode: # Button -- Seek Down
            if modePressed == 1:
                call(shlex.split('xbmc-send --action="Action(Right)"'))
            else:
                call(shlex.split('xbmc-send --action="PlayerControl(Previous)"'))

        if '213' in ardDecode: # Button -- Seek Down Long
            if modePressed == 1:
                call(shlex.split('xbmc-send --action="PlayerControl(PageDown)"'))
            else:
                call(shlex.split('xbmc-send --action="PlayerControl(Seek(-15))"'))
    
        if '100' in ardDecode: # The car is off, night night
            suspendSystem = Pyro4.Proxy("PYRONAME:system.suspend")
            print(suspendSystem.suspend_system_now(1))
    
        if '200' in ardDecode: # The car is on, Wakey Wakey
            resumeSystem = Pyro4.Proxy("PYRONAME:system.resume")
            print(resumeSystem.resume_system_now(0))
    
        #ardPort.flushInput() #I may or may not need this part
    
    except OSError as err:
        print("OS error: {0}".format(err))
