#!/bin/bash

cd /home/osmc/git/RPi3Carputer/
git pull origin master
sleep 3
cd /home/osmc/git/PythonScripting/
git pull origin master

echo "Done"
