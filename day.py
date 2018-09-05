#!/usr/bin/python3
import os

screenBrightnessFile = "/sys/class/backlight/rpi_backlight/brightness"

backlightFile = open(screenBrightnessFile, 'w')
backlightFile.write('100')
backlightFile.close()

exit()

