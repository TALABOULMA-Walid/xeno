
#include <linux/version.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

static dev_t          exemple_dev;
static struct cdev    exemple_cdev;
static struct class * exemple_class = NULL;

static int exemple_read (struct file * filp, char * buffer,
                          size_t length, loff_t * offset);

static int exemple_write(struct file * filp, const char * buffer,
                        size_t lg, loff_t * offset);

static struct file_operations fops_exemple = {
	.owner   =  THIS_MODULE,
	.read    =  exemple_read,
	.write   =  exemple_write,
};

#define NB_MINEURS 1

static int __init exemple_init (void)
{
	int erreur;
	
	erreur = alloc_chrdev_region(& exemple_dev,
	                             0, NB_MINEURS, THIS_MODULE->name);
	if (erreur < 0)
		return erreur;
		
	exemple_class = class_create(THIS_MODULE, "exemple");
	if (IS_ERR(exemple_class)) {
		unregister_chrdev_region(exemple_dev, NB_MINEURS);
		return -EINVAL;
	}
	device_create(exemple_class, NULL, exemple_dev,
	              NULL, THIS_MODULE->name);

	cdev_init(& exemple_cdev, & fops_exemple);

	erreur = cdev_add(& exemple_cdev, exemple_dev, NB_MINEURS);
	if (erreur != 0) {
		device_destroy(exemple_class, exemple_dev);
		class_destroy(exemple_class);
		unregister_chrdev_region(exemple_dev, NB_MINEURS);
		return erreur;
	}
	return 0; 
}



static void __exit exemple_exit (void)
{
	cdev_del(& exemple_cdev);
	device_destroy(exemple_class, exemple_dev);
	class_destroy(exemple_class);
	unregister_chrdev_region(exemple_dev, NB_MINEURS);
}



#define TAILLE_DATA_EXEMPLE  4096
static char data_exemple [TAILLE_DATA_EXEMPLE];
static int  lg_data_exemple = 0;
DEFINE_MUTEX(mtx_data_exemple);


static int exemple_read(struct file * filp, char * buffer,
                        size_t lg, loff_t * offset)
{
	if (mutex_lock_interruptible(& mtx_data_exemple) != 0)
		return -ERESTARTSYS;
	
	if (lg > lg_data_exemple)
		lg = lg_data_exemple;

	if (lg > 0) {
		if (copy_to_user(buffer, data_exemple, lg) != 0) {
			mutex_unlock(& mtx_data_exemple);
			return -EFAULT;
		}
		lg_data_exemple -= lg;
		if (lg_data_exemple > 0)
			memmove(data_exemple, & data_exemple[lg], lg_data_exemple);
	}
	mutex_unlock(& mtx_data_exemple);
	return lg;
}


static int exemple_write(struct file * filp, const char * buffer,
                        size_t lg, loff_t * offset)
{
	if (mutex_lock_interruptible(& mtx_data_exemple) != 0)
		return -ERESTARTSYS;
	
	if (lg > (TAILLE_DATA_EXEMPLE - lg_data_exemple))
		lg = TAILLE_DATA_EXEMPLE - lg_data_exemple;

	if (lg > 0) {
		if (copy_from_user(&data_exemple[lg_data_exemple],
		                   buffer, lg) != 0) {
			mutex_unlock(& mtx_data_exemple);
			return -EFAULT;
		}
		lg_data_exemple += lg;
	}
	mutex_unlock(& mtx_data_exemple);
	return lg;
}

module_init(exemple_init);
module_exit(exemple_exit);
MODULE_LICENSE("GPL");

