#!/usr/bin/python3

#############################################################
###  This just goes to the two git repos and does a pull
#############################################################

import os
import shlex
from subprocess import call

call(shlex.split('bash /home/osmc/git/RPi3Carputer/gitPull.sh'))

exit()
