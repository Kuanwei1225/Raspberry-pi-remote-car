#include <linux/module.h>	//module stuff
#include <linux/kernel.h>	//printk
#include <linux/init.h>		//__init
#include <linux/stat.h>		//permissions
#include <linux/device.h>	//Device class stuff
#include <linux/fs.h>		//File operation structures
#include <linux/err.h>		//Error checking macros
#include <linux/cdev.h>		//region char device

#include <linux/ioctl.h>
#include <linux/io.h>		//read and write from the memory
#include <asm/uaccess.h>	//translation from userspace ptr to kernelspace
#include <mach/platform.h>	//pull address of system timer
#include <linux/gpio.h>		//set gpio


#include <linux/types.h>	//uintxx_t

#include <linux/sched.h>	//for current->pid

#include "mygpio_ioctl.h"

#define MOD_NAME "rpigpio" 				//name of the device file
#define GPIO_LABEL "output"

static int gpio_open(struct inode*inode, struct file *filp);
static ssize_t gpio_write(struct file *filp, const char __user *buf,
									size_t count, loff_t *f_pos);
static long gpio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
//static int st_release(struct inode *inode, struct file *filp);

static struct file_operations gpio_fops = {
	.owner			= THIS_MODULE,
	.open			= gpio_open,
	.write			= gpio_write,
	.unlocked_ioctl	= gpio_ioctl,
	//.release 		= st_release,
	//.unlocked_ioctl = st_ioctl,
};

static int gpio_major = 0;           /*dynamic*/
static int gpio_minor = 0;
static int gpio_devs = 1;           /* device count */
static int gpio_pin = 4;            /*output pin*/
//static int open_count = 0;
static struct class *my_class = NULL;
static struct cdev gpio_cdev;
//static dev_t gpio_dev;
spinlock_t lock;
//volatile unsigned *gpio;
static int gpio_open(struct inode*inode, struct file *filp)
{
	return 0;
}

static ssize_t gpio_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	char wbuf[1];
	if (copy_from_user(wbuf, buf, 1)) {
		return -1;
	}
	if(wbuf[0] == '1')
		gpio_set_value(gpio_pin, 1);
	else
		gpio_set_value(gpio_pin, 0);
	return 0;
}
static long gpio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int gpio_val, pin, ret;
	struct gpio_data gdata;

	switch (cmd)
		{
			case GPIO_READ:
				get_user (pin, (int __user *) arg);
				gpio_val = gpio_get_value(pin);
				return gpio_val;
			case GPIO_WRITE:
				if  (copy_from_user(&gdata, (struct gpio_data __user *)arg
										, sizeof(gdata)) )
					return -1;
				spin_lock(&lock);
				if (gpio_direction_output(gdata.pin, gdata.data) )
					goto error;
				spin_unlock(&lock);
				return 0;
			case GPIO_REQUEST:
				get_user (pin, (int __user*)arg);
				spin_lock(&lock);
				ret = gpio_request(pin, "gpio_req");
				if (ret )
					goto error;
				ret = gpio_direction_input(pin);
				spin_unlock(&lock);
				return 0;
			case GPIO_FREE:
				get_user (pin, (int __user*)arg);
				spin_lock(&lock);
				gpio_free(pin);
				spin_unlock(&lock);
				return 0;
			case GPIO_MODE_SET:
				if  (copy_from_user(&gdata, (struct gpio_data __user *)arg
										, sizeof(gdata)) )
					return -1;
				spin_lock(&lock);
				if (gdata.mode == 'R' || gdata.mode == 'r') {
					if (gpio_direction_input(gdata.pin) )
						goto error;
				}else {
					if (gpio_direction_output(gdata.pin, 0) )
						goto error;
				}
				spin_unlock(&lock);
				return 0;
			case GPIO_SET:
				get_user (pin, (int __user *)arg);
				gpio_set_value(pin, 1);
				return 0;
			case GPIO_CLR:
				get_user (pin, (int __user *)arg);
				gpio_set_value(pin, 0);
				return 0;
		}
error:
	spin_unlock(&lock);
	return -1;
}
static int __init gpio_init(void)
{
	int alloc_ret = 0, cdev_err = 0, major;
	struct device *my_dev;
	dev_t dev = MKDEV(gpio_major, 0);

	printk(KERN_INFO " : Startup\n");
	//register char device
	alloc_ret = alloc_chrdev_region(&dev, 0, gpio_devs, MOD_NAME);//character device
	if (alloc_ret) {
		printk(KERN_ALERT " : Device Cannot Register\n");
		return -1;
	}
	gpio_major = major = MAJOR(dev);
	cdev_init(&gpio_cdev, &gpio_fops);
	gpio_cdev.owner = THIS_MODULE;
	gpio_cdev.ops = &gpio_fops;
	cdev_err = cdev_add(&gpio_cdev, MKDEV(gpio_major, gpio_minor), gpio_devs);
	if (cdev_err) {
		printk(KERN_ALERT " : cdev initial faild\n");
		goto error_i;
	}
	/*register class*/
	my_class = class_create(THIS_MODULE, "my_class");
	if (IS_ERR(my_class) ) {
		printk(KERN_ALERT " : Cannot creat class\n");
		goto error_i;
	}
	//Create Device
	//gpio_dev = MKDEV(gpio_major, gpio_minor);
	my_dev = device_create (
						my_class,
						NULL,
						MKDEV(gpio_major, 0),
						NULL, MOD_NAME );
	if (IS_ERR(my_dev)) {
		printk(KERN_ALERT " : Cannot create device\n");
		class_destroy(my_class);
		goto error_i;
	}
	spin_lock_init(&lock);
	return 0;
error_i:
	if (cdev_err == 0)
		cdev_del(&gpio_cdev);
	if (alloc_ret == 0)
		unregister_chrdev_region(dev, gpio_devs);
	return -1;
}

static void __exit gpio_exit(void)
{
	gpio_free(gpio_pin);
	device_destroy(my_class, MKDEV(gpio_major, 0));
	class_destroy(my_class);
	unregister_chrdev(gpio_major, MOD_NAME);

	printk(KERN_NOTICE " : Goodbye\n");
}
MODULE_LICENSE("Dual BSD/GPL");
module_init(gpio_init);
module_exit(gpio_exit);





