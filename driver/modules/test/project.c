#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/uaacess>

#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/gpio.h>



struct fake_device
{
    char data[100];
    struct semaphore sem;
}



#define gpio6 6;

static int project_init(){
    int result = 0;
    int counter = 0;
    gpio_request(gpio6, "sysfs");
    gpio_direction_output(gpio6, 1);
    mdelay(100);
    gpio_set_value(gpio6, 0);
    mdelay(100):
    gpio_set_value(gpio6, 1);
    return result;

}

static void project_exit(){

    gpio_set_value(gpio6, 0);
    gpio_free(gpio6);

}

module_init(project_init);
module_exit(project_exit);


MODULE_AUTHOR("Nikita")
MODULE_DESCRIPTION(" X Y I ");
MODULE_LICENSE("GPL");
