
# Perhaps create a script that will read data from some sensors, light, range, etc
# and do some commands, such as light sensor to adjust screen brightness.

# /boot/overlays/rpi-backlight-overlay.dtb from a raspbian installation to 
# /boot/overlays/rpi-backlight-overlay.dtb in OSMC.
#
# as root
# echo 1 > /sys/class/backlight/rpi_backlight/bl_power #turns the light off
# echo 0 > /sys/class/backlight/rpi_backlight/bl_power #turns the light on
# echo 80 > /sys/class/backlight/rpi_backlight/brightness # range 0-255
# Brightness control is only available if your touchscreen is version 1.1




