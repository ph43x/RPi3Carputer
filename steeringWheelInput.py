#! /usr/bin/python

# Steering Wheel Controls
# Read inputs from Arduino

from time import sleep
import RPi.GPIO as GPIO
from subprocess import call
from shlex import split
import datetime

GPIO.setmode(GPIO.BOARD)

turnoff = 31    #bcm pin 06
volDown = 38 	#bcm pin 20
volUp = 36		#bcm pin 16
seekDown = 37	#bcm pin 26
seekUp = 35		#bcm pin 19
buttonPower=33	#bcm pin 13
buttonMode=40	#bcm pin 21
currentVolume = 50 #value for keeping track of volume
buttomModePressed = 0 #keeping track if the Mode button is pressed
buttonModePressedStart = 0
buttonModePressedFinish = 0

#Set which pins GPIO is looking at for input
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
			#Ok, I wanted a function button so Mode will be it, it will auto turn off after 1 minute
			#or if pressed again within that time. This will allow other buttons to have improved functionality
			if GPIO.input(buttonMode) == 1 and buttonModePressed == 0:
				buttonModePressed = 1
				buttonModePressedStart = datetime.datetime.now()
				buttonModePressedFinish = buttonModePressedStart + datetime.timedelta(minutes = 1)
				sleep(.1)
			if GPIO.input(buttonMode) == 1 and buttonModePressed == 1:
				buttonModePressed = 0
				sleep(.1)
			if buttonModePressedStart > buttonModePressedFinish and buttonModePressed == 1:
				buttonModePressed = 0
			#Now that the mode selector is out of the way, we need to check if mode is pressed for everything
			if GPIO.input(volUp) == 1 and buttonModePressed == 0:
				if currentVolume >= 100:
					return;
				else:
					#run command for increasing volume in OSMC by 5%
					currentVolume = currentVolume + 5
					#since call was imported from subprocess there is no need to use subprocess.call just call
					#also since split was imported from shlex, again only split is used instead of shlex.split
					call(split('xbmc-send --host=127.0.0.1 --action="SetVolume(percent[$currentVolume])"'))
					sleep(.25)
		
			if GPIO.input(volDown) == 1 and buttonModePressed == 0:
				if currentVolume <= 0:
					return;
				else:
					#run command for decreasing volume in OSMC by 5%
					currentVolume = currentVolume - 5
					call(split('xbmc-send --host=127.0.0.1 --action="SetVolume(percent[$currentVolume])"')
					sleep(.25)
		
			#Now I would like to use Seek to skip tracks or skip folders. Not real sure how to do this yet
			if GPIO.input(seekUp) == 1 and buttonModePressed == 0:
				#run command for next track in OSMC
				call(split('xbmc-send --host=127.0.0.1 --action="PlayerControl(Next)"'))
				sleep(.25)

			if GPIO.input(seekDown) == 1 and buttonModePressed == 0:
				#run command for previous track in OSMC
				call(split('xbmc-send --host=127.0.0.1 --action="PlayerControl(Previous)"'))
				sleep(.25)
					     
# PlayMedia(media[,isdir][,1],[playoffset=xx]) seems to be the command i'm looking for
# I just need a way to find what the current directory ID is, and increment it
# Or http://kodi.wiki/view/Action_IDs , Action(action) , Action( [,window])
# also perhaps using a series of commands will achieve the same thing.
#call(split('xbmc-send --host=127.0.0.1 --action="Action(ParentDir[,filemanager])"'))
#call(split('xbmc-send --host=127.0.0.1 --action="Action(Down[,filemanager])"'))
#call(split('xbmc-send --host=127.0.0.1 --action="Action(Select[,filemanager])"'))
#call(split('xbmc-send --host=127.0.0.1 --action="Action(PlayPause[,filemanager])"'))

#call(split('xbmc-send --host=127.0.0.1 --action="Action(ParentDir[filemanager,window])"'))
#call(split('xbmc-send --host=127.0.0.1 --action="Action(Down[filemanager,window])"'))
#call(split('xbmc-send --host=127.0.0.1 --action="Action(Select[filemanager,window])"'))
#call(split('xbmc-send --host=127.0.0.1 --action="Action(Play[filemanager,window])"'))
			
			if GPIO.input(seekUp) == 1 and buttonModePressed == 1:
				#run command for next track in OSMC
				call(split('xbmc-send --host=127.0.0.1 --action="PlayerControl(Next)"'))
				sleep(.25)

			if GPIO.input(seekDown) == 1 and buttonModePressed == 1:
				#run command for previous track in OSMC
				call(split('xbmc-send --host=127.0.0.1 --action="PlayerControl(Previous)"'))
				sleep(.25)
		
			if GPIO.input(buttonPower) == 1:
				#run command for play in OSMC, play acts as play/pause
				call(split('xbmc-send --host=127.0.0.1 --action="PlayerControl(Play)"'))
				sleep(.25)

finally:
	GPIO.cleanup()
