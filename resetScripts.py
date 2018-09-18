#!/usr/bin/python3

#############################################################
###  This script is going to be resetting the two scripts and
###  one nameserver daemon that are supposed to be running
#############################################################

import shlex
from subprocess import call

call(shlex.split('bash /home/osmc/git/PythonScripting/restartPyroHost.sh'))

exit()
