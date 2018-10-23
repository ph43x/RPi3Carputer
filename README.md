# RPi3Carputer
Creating a program in python and a couple arduinos to create a replacement head unit for a 2006 350Z using OSMC loaded on a Raspberry Pi 3

Idle power usage is 0.29 amps. 

This is where all the shortcuts show up in your favorites menu
Kodi 17.6 with CarTouch-PC skin that I'll be tweaking
#vi ~/.kodi/userdata/favourites.xml

This is where you can enable/disable LAN/WAN devices on startup
#sudo vi /var/lib/connman/settings
You can enter your wifi info and dhcp settings here
#sudo vi /var/lib/connman/wifi_<someHashedAuthentication>_managed_psk/settings

The file above makes it less likely to need to be edited, 
  but I put my WiFi info in it anyway since i don't know what else reads it
#sudo vi /etc/wpa_supplicant/wpa_supplicant.conf

Now you should be able to 
#sudo ifconfig wlan0 up
Now you should have an IP address
#sudo ifconfig wlan0
