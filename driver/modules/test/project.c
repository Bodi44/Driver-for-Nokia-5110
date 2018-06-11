#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/gpio.h>


struct fake_device
{
	char data[100];
	struct semaphore sem;
} virtual_device;

struct cdev *mcdev; // my cdev
int major_number; // stores major number to extracted from dev_t using macro - $ mknod /directory/file c major_number
int return_value; // used to hold return values of function; kernel stack is small so we use other variable in order not to eat it all

dev_t dev_num; // will hold major number that kernel gives us; name--> appears in /proc/devices

int device_open(struct inode* inode, struct file* flip) {

	// only allows one process to open this device by using semaphore as mutex
	if (down_interruptible(&virtual_device.sem) != 0) {
		printk(KERN_ALERT "pchdevice: could not lock device during open");
		return -1;
	}

	printk(KERN_INFO "pchdevice: opened device");
	return 0;
}

ssize_t device_read(struct file* flip, char* buffStoreData, size_t buffCount, loff_t* curOffset) {

	// take data from kernel space (device) to user space (process)
	// copy_to_user (destination, source, sizeOfTransfer)

	printk(KERN_INFO "pchdevice: reading from device");
	return_value = raw_copy_to_user(buffStoreData, virtual_device.data, buffCount);

	return return_value;
}

ssize_t device_write(struct file* flip, const char* buffSourceData, size_t buffCount, loff_t* curOffset) {

	// send data from user to kernel
	// copy_from_user (destination, source, sizeOfTransfer)

	printk(KERN_INFO "pchdevice: writing to device");
	return_value = raw_copy_from_user(virtual_device.data, buffSourceData, buffCount);
	
	return return_value;
}

int device_close(struct inode* inode, struct file* flip) {

	// when calling up we release mutex that we obtained at device opening

	up(&virtual_device.sem);
	printk(KERN_INFO "pchdevice: closed device");

	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,    // prevent unloading of this module when operations are in use
	.open = device_open,      // points to the method to call when opening the device
	.release = device_close,  // points to the method to call when closing the device
	.write = device_write,    // points to the method to call when writing to the device
	.read = device_read       // points to the method to call when reading from the device
};

#define DEVICE_NAME "pchdevice"

#define gpio6 6

static int project_init(void){
  
	return_value = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);

	// when kernel returns negative number, then there is an error
	if (return_value < 0) {
		printk(KERN_ALERT "pchdevice: fallied to allocate a major number");
		return return_value; // propagate error
	}

	major_number = MAJOR(dev_num); // extracs major number and stores it in variable (MACRO)

	// pr_alert("pchdevice: entered device");

    printk(KERN_INFO "pchdevice: entered device, device major %d", major_number);
	// printk(KERN_ALERT "pchdevice: major number is %d",major_number);

	mcdev = cdev_alloc();       // create cdev structure and initialize mcdev
	mcdev->ops = &fops;         // structure file operation
	mcdev->owner = THIS_MODULE; 

	// add cdev to kernel
	// int cdev_add(struct cdev* dev, dev_t)

	return_value = cdev_add(mcdev, dev_num, 1);

	// always check for errors
	if (return_value < 0) {
		printk(KERN_ALERT "pchdevice: unable to add cdev to kernel");
		return return_value;
	}

	// initialize semaphore
	sema_init(&virtual_device.sem, 1);

    int result = 0;
    gpio_request(gpio6, "sysfs");
    gpio_direction_output(gpio6, 1);
    mdelay(100);
    gpio_set_value(gpio6, 0);
    mdelay(100);
    gpio_set_value(gpio6, 1);
    return result;

}

static void project_exit(void){

	cdev_del(mcdev);

	unregister_chrdev_region(dev_num, 1);

	gpio_set_value(gpio6, 0);
    gpio_free(gpio6);

	printk(KERN_INFO "pchdevice: unloaded module");

}

module_init(project_init);
module_exit(project_exit);


MODULE_AUTHOR("Nikita");
MODULE_DESCRIPTION(" X Y I ");
MODULE_LICENSE("GPL");
