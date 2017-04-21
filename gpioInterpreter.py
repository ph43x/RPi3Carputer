#! /usr/bin/python

# Steering Wheel Controls
# Read inputs from Arduino

from time import sleep
import RPi.GPIO as GPIO
from subprocess import call
from shlex import split
import datetime

#  am using the BOARD gpio settings, so this will be the pin number not how BCM sees it
# After the pins I have values for volume, if mode is pressed, and the time it was pressed
GPIO.setmode(GPIO.BOARD)
turnoff = 31
volDown = 38
volUp = 36
seekDown = 37
seekUp = 35
buttonPower = 33
buttonMode = 40
currentVolume = 50
modePressed = 0
modeStart = 0
modeFinish = 0

# Set which pins GPIO is looking at for input and putting them in a pull down state so they stay at 0 volts until triggered
GPIO.setup(turnoff, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(volDown, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(volUp, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(seekDown, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(seekUp, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(buttonPower, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(buttonMode, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

try:
    while True:
        if GPIO.input(turnoff) == 1:
            call(split('shutdown now'))
            # Ok, I wanted a function button so Mode will be it, it will auto turn off after 1 minute
            # or if pressed again within that time. This will allow other buttons to have improved functionality
        if GPIO.input(buttonMode) == 1 and modePressed == 0:
            modePressed = 1
            modeStart = datetime.datetime.now()
            modeFinish = modeStart + datetime.timedelta(minutes = 1)
            call(split('xbmc-send --action="ActivateWindow(favourites)"'))
            call(split('xbmc-send --action="Action(select)"'))
            sleep(0.1)
        if GPIO.input(buttonMode) == 1 and modePressed == 1:
            modePressed = 0
            call(split('xbmc-send --action="ActivateWindow(home)"'))
            sleep(0.1)
        if modeStart > modeFinish and modePressed == 1:
            modePressed = 0
            # Now that the mode selector is out of the way, we need to check if mode is pressed for everything
        if GPIO.input(volUp) == 1 and modePressed == 0:
            if currentVolume > 95:
                currentVolume = 100
            else:
                #run command for increasing volume in OSMC by 5%
                currentVolume = currentVolume + 5
                # since call was imported from subprocess there is no need to use subprocess.call just call
                # also since split was imported from shlex, again only split is used instead of shlex.split
                call(split('xbmc-send --action="SetVolume(percent[$currentVolume])"'))
                sleep(0.25)
        if GPIO.input(volDown) == 1 and modePressed == 0:
            if currentVolume < 5:
                currentVolume = 0
            else:
                #run command for decreasing volume in OSMC by 5%
                currentVolume = currentVolume - 5
                call(split('xbmc-send --action="SetVolume(percent[$currentVolume])"')
                sleep(0.25)
        if GPIO.input(volUp) == 1 and modePressed == 1:
            # run command for next track in OSMC
            call(split('xbmc-send --action="Action(up)"'))
            sleep(0.25)
        if GPIO.input(volDown) == 1 and modePressed == 1:
            # run command for previous track in OSMC
            call(split('xbmc-send --action="Action(down)"'))
            sleep(0.25)
        if GPIO.input(seekUp) == 1 and modePressed == 0:
            # run command for next track in OSMC
            call(split('xbmc-send --action="PlayerControl(Next)"'))
            sleep(0.25)
        if GPIO.input(seekDown) == 1 and modePressed == 0:
            # run command for previous track in OSMC
            call(split('xbmc-send --action="PlayerControl(Previous)"'))
            sleep(0.25)
        if GPIO.input(seekUp) == 1 and modePressed == 1:
            # run command for next track in OSMC
            call(split('xbmc-send --action="Action(ParentDir)"'))
            sleep(0.25)
        if GPIO.input(seekDown) == 1 and modePressed == 1:
            # run command for previous track in OSMC
            call(split('xbmc-send --action="Action(select)"'))
            sleep(0.25)
        if GPIO.input(buttonPower) == 1 and modePressed == 0:
            # run command for play in OSMC, play acts as play/pause
            call(split('xbmc-send --action="PlayerControl(Play)"'))
            sleep(0.25)
        if GPIO.input(buttonPower) == 1 and modePressed == 1:
            # run command for play in OSMC, play acts as play/pause
            call(split('xbmc-send --action="PlayerControl(PartyMode)"'))
            call(split('xbmc-send --action="Action(select)"'))
            sleep(0.25)
finally:
    GPIO.cleanup()
