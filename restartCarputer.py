#!/usr/bin/python3

#############################################################
###  Restart the system
#############################################################

import shlex
from subprocess import call

call(shlex.split('sudo shutdown -r now'))

exit()
