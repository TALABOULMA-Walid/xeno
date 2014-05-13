// ---------------------------------------------------------------------
// exemple-rtdm-read-write.c
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
#include <linux/fs.h>

#include <asm/uaccess.h>

#include <rtdm/rtdm_driver.h>

#define exemple_nom_driver "exemple_rtdm"
#define exemple_nom_device "rtdev_exemple"


static int exemple_read(struct rtdm_dev_context * contexte,
                        rtdm_user_info_t * info, 
                        void * buffer, size_t lg);
                        
static int exemple_write(struct rtdm_dev_context * contexte,
                        rtdm_user_info_t * info, 
                        const void * buffer, size_t lg);

static int exemple_open_nonrt (struct rtdm_dev_context * contexte, 
                               rtdm_user_info_t * info,
                               int flags);

static int exemple_close_nonrt (struct rtdm_dev_context * contexte, 
                                rtdm_user_info_t * info);


static struct rtdm_device exemple_rtdev = {
	.struct_version = RTDM_DEVICE_STRUCT_VER,
	.device_flags   = RTDM_NAMED_DEVICE,
	.context_size   = 0,
	.device_name    = exemple_nom_device,
	.open_nrt       = exemple_open_nonrt,
	.ops = {
		.close_nrt = exemple_close_nonrt,
		.read_nrt  = exemple_read,
		.write_nrt = exemple_write,
	},
	.device_class     = RTDM_CLASS_TESTING,
	.device_sub_class = 1,
	.profile_version  = 1,
	.driver_name      = exemple_nom_driver,
	.driver_version   = RTDM_DRIVER_VER(1,0,0),
	.peripheral_name  = exemple_nom_driver,
	.provider_name    = "cpb",
	.proc_name        = exemple_nom_driver,
};



#define TAILLE_DATA_EXEMPLE  4096
static char data_exemple [TAILLE_DATA_EXEMPLE];
static int  lg_data_exemple = 0;
static rtdm_mutex_t mtx_data_exemple;



static int __init exemple_init (void)
{
	rtdm_mutex_init(&mtx_data_exemple);
	rtdm_dev_register (& exemple_rtdev);
	return 0; 
}



static void __exit exemple_exit (void)
{
	rtdm_dev_unregister (& exemple_rtdev, 0);
	rtdm_mutex_destroy(&mtx_data_exemple);
}



static int exemple_read(struct rtdm_dev_context * contexte,
                        rtdm_user_info_t * info, 
                        void * buffer, size_t lg)
{
	rtdm_mutex_lock(& mtx_data_exemple);
	
	if (lg > lg_data_exemple)
		lg = lg_data_exemple;
	if (lg > 0) {
		if (rtdm_safe_copy_to_user(info, buffer, data_exemple, lg) != 0) {
			rtdm_mutex_unlock(& mtx_data_exemple);
			return -EFAULT;
		}
		lg_data_exemple -= lg;
		if (lg_data_exemple > 0)
			memmove(data_exemple, & data_exemple[lg], lg_data_exemple);
	}
	rtdm_mutex_unlock(& mtx_data_exemple);
	return lg;
}



static int exemple_write(struct rtdm_dev_context * contexte,
                        rtdm_user_info_t * info, 
                        const void * buffer, size_t lg)
{
	rtdm_mutex_lock(& mtx_data_exemple);
	
	if (lg > (TAILLE_DATA_EXEMPLE - lg_data_exemple))
		lg = TAILLE_DATA_EXEMPLE - lg_data_exemple;

	if (lg > 0) {
		if (rtdm_safe_copy_from_user(info,
		                  &data_exemple[lg_data_exemple],
		                  buffer, lg) != 0) {
			rtdm_mutex_unlock(& mtx_data_exemple);
			return -EFAULT;
		}
		lg_data_exemple += lg;
	}
	rtdm_mutex_unlock(& mtx_data_exemple);
	return lg;
}



static int exemple_open_nonrt (struct rtdm_dev_context * contexte, 
                               rtdm_user_info_t * info,
                               int flags) 
{
	return 0;
}



static int exemple_close_nonrt (struct rtdm_dev_context * contexte, 
                                rtdm_user_info_t * info) 
{
	return 0;
}



module_init(exemple_init);
module_exit(exemple_exit);
MODULE_LICENSE("GPL");

