
#include <linux/module.h>

static int __init exemple_squelette_init (void)
{
	printk(KERN_INFO "%s: chargement du module\n", THIS_MODULE->name);
	return 0; 
}
 
static void __exit exemple_squelette_exit (void)
{
	printk(KERN_INFO "%s: retrait du module\n", THIS_MODULE->name);
}

module_init(exemple_squelette_init);
module_exit(exemple_squelette_exit);

MODULE_LICENSE("GPL");

