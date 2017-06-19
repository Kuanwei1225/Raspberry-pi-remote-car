#ifndef _MYGPIO_IOCTL_H
#define _MYGPIO_IOCTL_H

#include <linux/ioctl.h> 

#define IO_MAGIC 'M'
struct gpio_data {
	int pin;
	int data;
	char mode;
};
// in : pin to read  //return : the value on the pin
#define GPIO_READ  _IOR(IO_MAGIC, 1, int)
// in : struct (pin, data) // return : success or not
#define GPIO_WRITE  _IOW(IO_MAGIC, 2, struct gpio_data)
// in : pin to request and set to input pin // return : success or not
#define GPIO_REQUEST _IOW(IO_MAGIC, 3, int)
// in : pin to free
#define GPIO_FREE _IOW(IO_MAGIC, 4, int)
// in : struct (pin, mode[R/W])
#define GPIO_MODE_SET  _IOW(IO_MAGIC, 5, struct gpio_data)
// in : pin to set ( write 1 )
#define GPIO_SET  _IOW(IO_MAGIC, 6, int)
// in : pin to clear ( write 0 )
#define GPIO_CLR  _IOW(IO_MAGIC, 7, int)

#endif
