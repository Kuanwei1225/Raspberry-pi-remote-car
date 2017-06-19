sudo insmod mygpio_ioctl.ko
sudo chmod 666 /dev/rpigpio
spawn-fcgi -a127.0.0.1 -p8081 -n ./cgi/gpioCgi &

gst-launch-1.0 -v v4l2src ! 'video/x-raw, width=640, height=480, framerate=30/1' ! queue ! videoconvert ! omxh264enc !  h264parse ! flvmux ! rtmpsink location='rtmp://localhost/rtmp/live live=1' &
