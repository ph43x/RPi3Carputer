import picamera
import time
import datetime
import os

camera = picamera.PiCamera()
videoTemp = '/home/osmc/camera/temp/'
videoSaved = '/home/osmc/camera/video/'
picSaved = '/home/osmc/camera/pic/'
i = 0

#Camera settings with default values
camera.sharpness = 0
#camera.contrast = 0
camera.contrast = 50
#camera.brightness = 50
camera.brightness = 75
camera.saturation = 0
camera.ISO = 0
camera.video_stabilization = False
camera.exposure_compensation = 0
camera.exposure_mode = 'auto'
camera.meter_mode = 'average'
camera.awb_mode = 'auto'
camera.image_effect = 'none'
camera.color_effects = None
camera.rotation = 0
camera.hflip = False
#camera.vflip = False
camera.vflip = True
camera.crop = (0.0, 0.0, 1.0, 1.0)

def start_recording():
  fileName = os.path.join(videoTemp, datetime.datetime.now().strftime('%Y-%m-%d_%H.%M.%S.h264'))
  camera.start_preview()
  #camera.start_recording(fileName)

def stop_recording():
  #camera.stop_recording()
  camera.stop_preview()

def take_picture():
  fileName = str(datetime.date.today())
  camera.capture(fileName)

def save_video():
  return

#while i < 5:
#  start_recording()
#  time.sleep(30)
#  stop_recording()
#  i =+ 1
#  print(i)

def start():
  action = input('(B)rightness? 1-100 Or (Start) Preview and (Stop) Preview. Also (Exit)\n')

  if action == 'B' or action == 'b':
    action2 = 0
    action2 = input('Brightness level? 1-100\n')

    if int(action2) > 0 and int(action2) < 100:
      camera.brightness = int(action2)
      print('Brightness changed to ', int(action2))
      return(start())
    else:
      print('Value not in range 1-100\n')
      return(start())

  if action == 'Start' or action == 'start':
    start_recording()
    #time.sleep(3)
    #camera.contrast = 25
    #time.sleep(3)
    #camera.brightness = 75
    #camera.contrast = 50
    #time.sleep(3)
    #camera.contrast = 75
    return(start())
    
  if action == 'Stop' or action == 'stop':
    stop_recording()
    return(start())

  if action == 'Exit' or action == 'exit':
    exit()

  else:
    print('Entry not valid, try again.. stupid.')
    return(start())

print(start())

exit()
