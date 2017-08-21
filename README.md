# Raspberry-pi-remote-car
A remote car by using Raspberry pi

# Abstract
  This simple project is about a remote car. Writing Linux driver to control GPIO and the motor. Set up a webserver on Raspberry pi, and people using wifi connect the website by browser. The website will show the webcam stream and user also can give order to control the car.</br>

#  System Architecture
This project include webcam, raspberry pi, H bridge motor drive and DC motor.  Set up a website as a GUI interface, and Using webcam to catch the stream and show on the webside. Writing linux driver to control the GPIO, and further, control the DC motor.</br>
<img src="https://github.com/Kuanwei1225/Raspberry-pi-remote-car/blob/master/image/system_block.jpg"></br>

This project use website as a GUI interface. Write HTML code post command to background decode command, and call system call. System call will control GPIO.</br>
<img src="https://github.com/Kuanwei1225/Raspberry-pi-remote-car/blob/master/image/sw_sys_block.jpg"></br>

# Resault
The simple website shoe as below. Right window show the webcam stream, and left window show the command block.</br>
<img src="https://github.com/Kuanwei1225/Raspberry-pi-remote-car/blob/master/image/final.jpg" width = "70%" height = "70%"></br>
Test the webcam stream.
<img src="https://github.com/Kuanwei1225/Raspberry-pi-remote-car/blob/master/image/camara.jpg" width = "70%" height = "70%"></br>
Remote car.
<img src="https://github.com/Kuanwei1225/Raspberry-pi-remote-car/blob/master/image/car.jpg" width = "70%" height = "70%"></br>
