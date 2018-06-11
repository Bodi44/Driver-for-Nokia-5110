#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/delay.h>



#define gpio6 = 6;

static int project_init(){
    int result = 0;
    int counter = 0;
    gpio_request(gpio6, "sysfs");
    gpio_direction_output(gpio6, 1);
    mdelay(100);
    for(counter; counter < 5; counter++){
    	gpio_set_value(gpio6, 0);
    	mdelay(100):
    	gpio_set_value(gpio6, 1);
    	mdelay(100):
    }
    return result;

}

static void project_exit(){

    gpio_set_value(gpio6, 0);
    gpio_free(gpio6);

}

module_init(project_init);
module_exit(project_exit);


MODULE_LICENSE("GPL")