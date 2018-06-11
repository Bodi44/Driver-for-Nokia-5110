#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

static unsigned int gpio17 = 17;
static bool pinOFF = 0;
static bool pinON = 1;

static int project_init(){

    int result = 0;
    gpio_direction_output(gpio17, pinON);
    return result;

}

static void project_exit(){

    gpio_set_value(gpio17, pinOFF);
    gpio_free(gpio17);

}

module_init(project_init);
module_exit(project_exit);
