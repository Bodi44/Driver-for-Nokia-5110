#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>


struct fake_device
{
	char data[100];
	struct semaphore sem;
} virtual_device;


struct cdev *mcdev;

int major_number;
int return_value;

dev_t dev_num;

#define DEVICE_NAME "lcd_module"


int device_open(struct inode *inode, struct file *filp)
{
	if (down_interruptible(&virtual_device.sem) != 0)
	{
		printk(KERN_INFO "lcd_module: could not lock devic during open\n");
		return -1;
	}

	printk(KERN_INFO "lcd_module: opened device\n");
	return 0;
}


int device_close(struct inode* inode, struct file* filp) 
{
	up(&virtual_device.sem);
	printk(KERN_DEBUG "pchdevice: closed device\n");
	return 0;
}


ssize_t device_read(struct file* filp, char* buffStoreData, size_t buffCount, loff_t* curOffset) 
{
	printk(KERN_DEBUG "pchdevice: reading from device\n");
	return_value = raw_copy_to_user(buffStoreData,virtual_device.data,buffCount);
	return return_value;
}


ssize_t device_write(struct file* filp, const char* buffSourceData, size_t buffCount, loff_t* curOffset) 
{
	printk(KERN_DEBUG "pchdevice: writing to device\n");
	return_value = raw_copy_from_user(virtual_device.data,buffSourceData,buffCount);
	return return_value;
}


struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_close,
	.write = device_write,
	.read = device_read
};


static int driver_entry(void)
{
	return_value = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);

	if (return_value < 0) 
	{		
		printk(KERN_ALERT "lcd_module: falied to allocate major number\n");
		return return_value;
	}

	major_number = MAJOR(dev_num);

	printk(KERN_INFO "lcd_module: use \"mknod /dev/%s c %d 0\" for device file\n",DEVICE_NAME,major_number);

	mcdev = cdev_alloc();
	mcdev->ops = &fops;
	mcdev->owner = THIS_MODULE;

	return_value = cdev_add(mcdev, dev_num, 1);

	if (return_value < 0) 
	{
		printk(KERN_ALERT "lcd_module: unable to add cdev to kernel\n");
		return return_value;
	}

	sema_init(&virtual_device.sem,1);

	return return_value;
}

static void driver_exit(void) 
{
	cdev_del(mcdev);

	unregister_chrdev_region(dev_num, 1);

	printk(KERN_ALERT "lcd_module: unloaded module\n");
}

module_init(driver_entry);
module_exit(driver_exit);

MODULE_AUTHOR("lcd_module");
MODULE_DESCRIPTION("this is driver");
MODULE_LICENSE("GPL");
