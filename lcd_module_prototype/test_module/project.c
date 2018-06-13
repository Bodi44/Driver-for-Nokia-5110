#include <linux/module.h>
#include <linux/kernel.h>


#define DEVICE_NAME "lcd_module"


static int driver_entry(void)
{
	

	printk(KERN_ALERT "lcd_module: loaded module\n");

	return 0;
}

static void driver_exit(void) 
{
	

	printk(KERN_ALERT "lcd_module: unloaded module\n");
}

module_init(driver_entry);
module_exit(driver_exit);

MODULE_AUTHOR("lcd_module");
MODULE_DESCRIPTION("this is driver");
MODULE_LICENSE("GPL");
