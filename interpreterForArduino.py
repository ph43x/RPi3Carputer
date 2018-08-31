#! /usr/bin/python

# Steering Wheel Controls
# Read inputs from Arduino via serial over USB

import time
import RPi.GPIO as GPIO
from subprocess import call
import shlex
import datetime
import serial

# using the BOARD gpio settings, so this will be the pin number not how BCM
# sees it. After the pins I have values for volume, if mode is pressed, and
# the time it was pressed
GPIO.setmode(GPIO.BOARD)
turnoff = 18
volDown = 33
volUp = 36
seekDown = 35
seekUp = 38
buttonPower = 37
buttonMode = 40
currentVolume = 50
modePressed = 0
modeStart = 0
modeFinish = 0
pressedTime = 2
lastPressed = 0
lastTimePressed = time.time.now()
modeTimePressed = time.time.now()

# Enable anything to be able to read the arduino usb
call(shlex.split('sudo chmod 777 /dev/ttyUSB0'))
time.sleep(3.0)
ardPort = serial.Serial(port='/dev/ttyUSB0', baudrate=9600) # Which usb port is connected to the arduino
time.sleep(1.0)

try: 
    ardPort.open()
  except Exception, e:
    print "error opening serial port: " + str(e)
    exit()

if ardPort.isOpen():
    try:
      ardPort.flushInput()
      ardPort.flushOutput()
# is this possible to require a flush input in the while loop to prevent false positives 
# or reading a button as pressed when it is not
    while True:
        readArd = ardPort.read()

    # Wire 1: Short: Mode,  165 ohms: Volume Up,   652 ohms: Seek Up
    # Wire 2: Short: Power, 165 ohms: Volume Down, 652 ohms: Seek Down
        if (modeTimePressed - time.time.now()) > 60:
            modePressed = 0
        if 101 in readArd: # Button -- Mode 
            # This checks how long the button 
            if lastPressed is 101 and pressedTime > (lastTimePressed - time.time.now()): 
                modePressed = 1
                modeTimePressed = time.time.now() 
            if (int(firstTimePressed) - int(lastTimePressed)) > pressedTime

            else:
                lastPressed = 101

                firstTimePressed = 
                lastTimePressed = time.time.now()

        if 102 in readArd: # Button -- Volume Up
            if lastPressed is 102 and pressedTime > (lastTimePressed - time.time.now()): 
                modePressed = 1
                modeTimePressed = time.time.now() 
            else:
                lastPressed = 102
                lastTimePressed = time.time.now()

        if 103 in readArd: # Button -- Seek Up


        if 201 in readArd: # Button -- Power

            
        if 202 in readArd: # Button -- Volume Down


        if 203 in readArd: # Button -- Seek Down


        if 111 in readArd: # The car is off, night night


        if 222 in readArd: # The car is on, Wakey Wakey



        if GPIO.input(turnoff) == 1:
            call(shlex.split('shutdown now'))
            # Ok, I wanted a function button so Mode will be it, it will auto
            # turn off after 1 minute or if pressed again within that time.
            # This will allow other buttons to have improved functionality
        if GPIO.input(buttonMode) == 1 and modePressed == 0:
            modePressed = 1
            modeStart = datetime.datetime.now()
            modeFinish = modeStart + datetime.timedelta(minutes = 1)
            call(shlex.split('xbmc-send --action="ActivateWindow(favourites)"'))
            call(shlex.split('xbmc-send --action="Action(select)"'))
            time.sleep(0.1)
        if GPIO.input(buttonMode) == 1 and modePressed == 1:
            modePressed = 0
            call(shlex.split('xbmc-send --action="ActivateWindow(home)"'))
            time.sleep(0.1)
        if modeStart > modeFinish and modePressed == 1:
            modePressed = 0
            # Volume controls, checks to make sure the currentVolume will not
            # go outside the range of 0-100
        if GPIO.input(volUp) == 1 and modePressed == 0 and currentVolume > 95: 
            currentVolume = 100
        if GPIO.input(volUp) == 1 and modePressed == 0 and currentVolume < 96:
            currentVolume = currentVolume + 5
            call(shlex.split('xbmc-send --action="SetVolume(percent[$currentVolume])"'))
            time.sleep(0.25)
        if GPIO.input(volDown) == 1 and modePressed == 0 and currentVolume < 5:
            currentVolume = 0
        if GPIO.input(volDown) == 1 and modePressed == 0 and currentVolume > 4:
            currentVolume = currentVolume - 5
            call(shlex.split('xbmc-send --action="SetVolume(percent[$currentVolume])"'))
            time.sleep(0.25)
        # Secondary Volume controls and secondary seek controls control folder navigation
        if GPIO.input(volUp) == 1 and modePressed == 1:
            # run command for next track in OSMC
            call(shlex.split('xbmc-send --action="Action(up)"'))
            time.sleep(0.25)
        if GPIO.input(volDown) == 1 and modePressed == 1:
            # run command for previous track in OSMC
            call(shlex.split('xbmc-send --action="Action(down)"'))
            time.sleep(0.25)
        if GPIO.input(seekUp) == 1 and modePressed == 0:
            # run command for next track in OSMC
            call(shlex.split('xbmc-send --action="PlayerControl(Next)"'))
            time.sleep(0.25)
        if GPIO.input(seekDown) == 1 and modePressed == 0:
            # run command for previous track in OSMC
            call(shlex.split('xbmc-send --action="PlayerControl(Previous)"'))
            time.sleep(0.25)
        if GPIO.input(seekUp) == 1 and modePressed == 1:
            # run command for next track in OSMC
            call(shlex.split('xbmc-send --action="Action(ParentDir)"'))
            time.sleep(0.25)
        if GPIO.input(seekDown) == 1 and modePressed == 1:
            # run command for previous track in OSMC
            call(shlex.split('xbmc-send --action="Action(select)"'))
            time.sleep(0.25)
        if GPIO.input(buttonPower) == 1 and modePressed == 0:
            # run command for play in OSMC, play acts as play/pause
            call(shlex.split('xbmc-send --action="PlayerControl(Play)"'))
            time.sleep(0.25)
        if GPIO.input(buttonPower) == 1 and modePressed == 1:
            # run command for play in OSMC, play acts as play/pause
            call(shlex.split('xbmc-send --action="PlayerControl(PartyMode)"'))
            call(shlex.split('xbmc-send --action="Action(select)"'))
            time.sleep(0.25)

else:
    print "cannot open serial port "

finally:
    GPIO.cleanup()
