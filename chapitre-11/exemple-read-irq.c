
#include <linux/interrupt.h>
#include <linux/version.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#include <asm/uaccess.h>


static int numero_irq = 1;
module_param(numero_irq, int, 0444);

static dev_t          exemple_dev;
static struct cdev    exemple_cdev;
static struct class * exemple_class = NULL;

static int exemple_read (struct file * filp, char * buffer,
                          size_t length, loff_t * offset);

static irqreturn_t exemple_handler(int irq, void * ident);

static struct file_operations fops_exemple = {
	.owner   =  THIS_MODULE,
	.read    =  exemple_read,
};

#define TAILLE_DATA_EXEMPLE  4096
static long int data_exemple [TAILLE_DATA_EXEMPLE];
static int  lg_data_exemple = 0;
static spinlock_t  spl_data_exemple;
static DECLARE_WAIT_QUEUE_HEAD(wq_data_exemple);

#define NB_MINEURS 1

static int __init exemple_init (void)
{
	int erreur;
	
	erreur = alloc_chrdev_region(& exemple_dev,
	                             0, NB_MINEURS, THIS_MODULE->name);
	if (erreur < 0)
		return erreur;
		
	exemple_class = class_create(THIS_MODULE, "classe_exemple");
	if (IS_ERR(exemple_class)) {
		unregister_chrdev_region(exemple_dev, NB_MINEURS);
		return -EINVAL;
	}
	device_create(exemple_class, NULL, exemple_dev,
	              NULL, THIS_MODULE->name);

	spin_lock_init(& spl_data_exemple);

	cdev_init(& exemple_cdev, & fops_exemple);

	erreur = cdev_add(& exemple_cdev, exemple_dev, NB_MINEURS);
	if (erreur != 0) {
		device_destroy(exemple_class, exemple_dev);
		class_destroy(exemple_class);
		unregister_chrdev_region(exemple_dev, NB_MINEURS);
		return erreur;
	}
	erreur = request_irq(numero_irq, exemple_handler,
	                  IRQF_SHARED, THIS_MODULE->name, THIS_MODULE->name);
	if (erreur != 0) {
		cdev_del(& exemple_cdev);
		device_destroy(exemple_class, exemple_dev);
		class_destroy(exemple_class);
		unregister_chrdev_region(exemple_dev, NB_MINEURS);
		return erreur;
	}
	return 0; 
}



static void __exit exemple_exit (void)
{
	free_irq(numero_irq, THIS_MODULE->name);
	cdev_del(& exemple_cdev);
	device_destroy(exemple_class, exemple_dev);
	class_destroy(exemple_class);
	unregister_chrdev_region(exemple_dev, NB_MINEURS);
}



static int exemple_read(struct file * filp, char * buffer,
                        size_t length, loff_t * offset)
{
	char chaine[80];
	unsigned long masque;

	spin_lock_irqsave(& spl_data_exemple, masque);

	while (lg_data_exemple == 0) {
		spin_unlock_irqrestore(& spl_data_exemple, masque);
		if (wait_event_interruptible(wq_data_exemple,
		                    (lg_data_exemple != 0)) != 0) {
			printk(KERN_INFO "Sortie sur signal\n");
			return -ERESTARTSYS;
		}
		spin_lock_irqsave(& spl_data_exemple, masque);
	}
	
	snprintf(chaine, 80, "%ld\n", data_exemple[0]);
	if (length < (strlen(chaine)+1)) {
		spin_unlock_irqrestore(& spl_data_exemple, masque);
		return -ENOMEM;
	}

	lg_data_exemple --;
	if (lg_data_exemple > 0)
		memmove(data_exemple, & (data_exemple[1]), lg_data_exemple * sizeof(long int));
	
	spin_unlock_irqrestore(& spl_data_exemple, masque);

	if (copy_to_user(buffer, chaine, strlen(chaine)+1) != 0)
		return -EFAULT;
	return strlen(chaine)+1;
}



static irqreturn_t exemple_handler(int irq, void * ident)
{
	spin_lock(& spl_data_exemple);
	if (lg_data_exemple < TAILLE_DATA_EXEMPLE) {
		data_exemple[lg_data_exemple] = jiffies;
		lg_data_exemple ++;
	}
	spin_unlock(& spl_data_exemple);
	wake_up_interruptible(& wq_data_exemple);
	return IRQ_HANDLED;
}


module_init(exemple_init);
module_exit(exemple_exit);
MODULE_LICENSE("GPL");

