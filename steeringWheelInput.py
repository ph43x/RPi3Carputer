#! /usr/bin/python

# Steering Wheel Controls
# Read inputs from Arduino

from time import sleep
import RPi.GPIO as GPIO
from subprocess import call
from shlex import split

GPIO.setmode(GPIO.BOARD)

turnoff = 31    #bcm pin 06
volDown = 38 	#bcm pin 20
volUp = 36		#bcm pin 16
seekDown = 37	#bcm pin 26
seekUp = 35		#bcm pin 19
buttonPower=33	#bcm pin 13
buttonMode=40	#bcm pin 21
currentVolume = 50 #value for keeping track of volume

#Set which pins GPIO is looking at for input
GPIO.setup(turnoff, GPIO.IN)
GPIO.setup(volDown, GPIO.IN)
GPIO.setup(volUp, GPIO.IN)
GPIO.setup(seekDown, GPIO.IN)
GPIO.setup(seekUp, GPIO.IN)
GPIO.setup(buttonPower, GPIO.IN)
GPIO.setup(buttonMode, GPIO.IN)

while(1):
	if GPIO.input(turnoff) == 1:
		call(split('shutdown now'))

	if GPIO.input(volUp) == 1:
		if currentVolume >= 100:
			return;
		else:
			#run command for increasing volume in OSMC by 5%
			currentVolume = currentVolume + 5
			#since call was imported from subprocess there is no need to use subprocess.call just call
			#also since split was imported from shlex, again only split is used instead of shlex.split
			call(split('xbmc-send --host=127.0.0.1 --action="SetVolume(percent[$currentVolume])"'))
			sleep(.25)
		
	if GPIO.input(volDown) == 1:
		if currentVolume <= 0:
			return;
		else:
			#run command for decreasing volume in OSMC by 5%
			currentVolume = currentVolume - 5
			call(split('xbmc-send --host=127.0.0.1 --action="SetVolume(percent[$currentVolume])"')
			sleep(.25)
		
	if GPIO.input(seekUp) == 1:
		#run command for next track in OSMC
		call(split('xbmc-send --host=127.0.0.1 --action="PlayerControl(Next)"'))
		sleep(.25)

	if GPIO.input(seekDown) == 1:
		#run command for previous track in OSMC
		call(split('xbmc-send --host=127.0.0.1 --action="PlayerControl(Previous)"'))
		sleep(.25)
		
	if GPIO.input(buttonPower) == 1:
		#run command for play in OSMC, play acts as play/pause
		call(split('xbmc-send --host=127.0.0.1 --action="PlayerControl(Play)"'))
		sleep(.25)

	if GPIO.input(buttonMode) == 1:
		#run command for stop in OSMC
		call(split('xbmc-send --host=127.0.0.1 --action="PlayerControl(Partymode)"'))
		sleep(.25)

GPIO.cleanup()
