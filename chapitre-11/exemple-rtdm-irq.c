// ---------------------------------------------------------------------
// exemple-rtdm-irq.c
// Fichier d'exemple du livre "Solutions Temps-Reel sous Linux"
// (C) 2012 - Christophe BLAESS
// http://christophe.blaess.fr
// ---------------------------------------------------------------------

#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/fs.h>

#include <asm/uaccess.h>

#include <rtdm/rtdm_driver.h>


static rtdm_irq_t irq_exemple;
static int exemple_handler(rtdm_irq_t * irq);

#define EXEMPLE_GPIO_IN  168
#define EXEMPLE_GPIO_OUT 183

static int __init exemple_init (void)
{
	int err;
	int num_irq = gpio_to_irq(EXEMPLE_GPIO_IN);
	
	if ((err = gpio_request(EXEMPLE_GPIO_IN, THIS_MODULE->name)) != 0) {
		return err;
	}
	if ((err = gpio_direction_input(EXEMPLE_GPIO_IN)) != 0) {
		gpio_free(EXEMPLE_GPIO_IN);
		return err;
	}
	if ((err = gpio_request(EXEMPLE_GPIO_OUT, THIS_MODULE->name)) != 0) {
		gpio_free(EXEMPLE_GPIO_IN);
		return err;
	}
	if ((err = gpio_direction_output(EXEMPLE_GPIO_OUT, 1)) != 0) {
		gpio_free(EXEMPLE_GPIO_OUT);
		gpio_free(EXEMPLE_GPIO_IN);
		return err;
	}

	if ((err = rtdm_irq_request(& irq_exemple, 
	                 num_irq, exemple_handler, 
	                 0,
	                 "Exemple", NULL)) != 0) {
		gpio_free(EXEMPLE_GPIO_OUT);
		gpio_free(EXEMPLE_GPIO_IN);
		return err;
	}
	set_irq_type(num_irq, IRQF_TRIGGER_RISING);
	rtdm_irq_enable(& irq_exemple);
	return 0; 
}



static void __exit exemple_exit (void)
{
	rtdm_irq_disable(& irq_exemple);
	rtdm_irq_free(& irq_exemple);
	gpio_free(EXEMPLE_GPIO_OUT);
	gpio_free(EXEMPLE_GPIO_IN);
}


static int exemple_handler(rtdm_irq_t * irq)
{
	static int value = 0;
	gpio_set_value(EXEMPLE_GPIO_OUT, value);
	value = 1 - value;
	return RTDM_IRQ_HANDLED;
}


module_init(exemple_init);
module_exit(exemple_exit);
MODULE_LICENSE("GPL");

