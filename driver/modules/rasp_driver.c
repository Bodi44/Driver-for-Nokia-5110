#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h> // file operation structure, allows use of open/close, read/write to device
#include <linux/cdev.h> // char driver; makes cdev avliable
#include <linux/semaphore.h> // used to access semaphore; synchronization behaviors
#include <asm/uaccess.h> // copy_to_user; copy_from_user;

// #include <linux/moduleparam.h> // for using parameters


// (1) Create a structure for our fake device
struct fake_device
{
	char data[100];
	struct semaphore sem;
} virtual_device;

// (2) To later register device we need cdev object and some other variables

struct cdev *mcdev; // my cdev
int major_number; // stores major number to extracted from dev_t using macro - $ mknod /directory/file c major_number
int return_value; // used to hold return values of function; kernel stack is small so we use other variable in order not to eat it all

dev_t dev_num; // will hold major number that kernel gives us; name--> appears in /proc/devices

#define DEVICE_NAME "pchdevice"

// (7) Called on device_file open
// inode reference to the file on disk and contains information about that file
// struct file represents an abstract open file
int device_open(struct inode* inode, struct file* flip) {

	// only allows one process to open this device by using semaphore as mutex
	if (down_interruptible(&virtual_device.sem) != 0) {
		printk(KERN_ALERT "pchdevice: could not lock device during open");
		return -1;
	}

	printk(KERN_INFO "pchdevice: opened device");
	return 0;
}

// (8) Called when user wants to read from device
ssize_t device_read(struct file* flip, char* buffStoreData, size_t buffCount, loff_t* curOffset) {

	// take data from kernel space (device) to user space (process)
	// copy_to_user (destination, source, sizeOfTransfer)

	printk(KERN_INFO "pchdevice: reading from device");
	return_value = raw_copy_to_user(buffStoreData, virtual_device.data, buffCount);

	return return_value;
}

// (9) Called when user wants to write to the device
ssize_t device_write(struct file* flip, const char* buffSourceData, size_t buffCount, loff_t* curOffset) {

	// send data from user to kernel
	// copy_from_user (destination, source, sizeOfTransfer)

	printk(KERN_INFO "pchdevice: writing to device");
	return_value = raw_copy_from_user(virtual_device.data, buffSourceData, buffCount);
	
	return return_value;
}

// (10) Called upon user closing
int device_close(struct inode* inode, struct file* flip) {

	// when calling up we release mutex that we obtained at device opening

	up(&virtual_device.sem);
	printk(KERN_INFO "pchdevice: closed device");

	return 0;
}



// (6) Tell kernel which functions to call when user operates on our device's file

struct file_operations fops = {
	.owner = THIS_MODULE,    // prevent unloading of this module when operations are in use
	.open = device_open,      // points to the method to call when opening the device
	.release = device_close,  // points to the method to call when closing the device
	.write = device_write,    // points to the method to call when writing to the device
	.read = device_read       // points to the method to call when reading from the device
};

static int driver_entry(void) {

	// (3) Register device with the system; a two step process
	
	// (step one)

	// use dynamic allocation to assign to device
	// a major number-- alloc_chrdev_region(dev_t*, uint fminor, uint count, char* name)

	return_value = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);

	// when kernel returns negative number, then there is an error
	if (return_value < 0) {
		printk(KERN_ALERT "pchdevice: fallied to allocate a major number");
		return return_value; // propagate error
	}

	major_number = MAJOR(dev_num); // extracs major number and stores it in variable (MACRO)

	printk(KERN_INFO "pchdevice: major_number is %d", major_number);
	printk(KERN_INFO "\tuse \"mknod /dev/%s c %d 0\" for device file", DEVICE_NAME, major_number);

	// (step one)

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

	// (4) Initialize semaphore
	sema_init(&virtual_device.sem, 1); // init value of one

	return 0;
}

static void driver_exit(void) {
	// (5) Unregister everything in reverse order
	// (step one)

	cdev_del(mcdev);

	// (step two)

	unregister_chrdev_region(dev_num, 1);
	printk(KERN_ALERT "pchdevice: unloaded module");
}


// Inform kernel where to start and stop
module_init(driver_entry);
module_exit(driver_exit);

MODULE_AUTHOR("pch");
MODULE_DESCRIPTION("Test module");
MODULE_LICENSE("GPL"); // general public license
