MODULE=myGPIO_ioctl
CFILES = myGPIO_ioctl.c
CCPREFIX=/home/k/rpi/tools/arm-bcm2708/arm-bcm2708-linux-gnueabi/bin/arm-bcm2708-linux-gnueabi-
KERNEL_SRC=/home/k/linux/


obj-m += myGPIO_ioctl.o
myGPIO_ioctl-objs := $(CFILES:.c=.o)
module_upload=myGPIO_ioctl.ko

all: clean compile

compile:
	make ARCH=arm CROSS_COMPILE=${CCPREFIX} -C ${KERNEL_SRC} M=$(PWD) modules

clean:
	make -C ${KERNEL_SRC} M=$(PWD) clean
  