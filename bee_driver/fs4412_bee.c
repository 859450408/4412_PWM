#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#include "fs4412_bee.h"

MODULE_LICENSE("Dual BSD/GPL");

#define BEE_MA 503 //主设备号
#define BEE_MI 0 //次设备号
#define BEE_NUM 1


#define FS4412_GPX1CON	0x11000C20
#define FS4412_GPX1DAT	0x11000C24





static unsigned int *gpx1con;
static unsigned int *gpx1dat;



struct cdev cdev;




static int bee_open(struct inode *inode, struct file *file)
{
	return 0;
}
	
static int bee_release(struct inode *inode, struct file *file)
{
	return 0;
}
	
static long bee_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

	switch (cmd) {
		case BEE_ON:
			printk("bee_on\n");
            writel(readl(gpx1dat) | 1 << 5, gpx1dat);
			break;
		case BEE_OFF:
			printk("bee_off\n");
            writel(readl(gpx1dat) & ~(1 << 5), gpx1dat);
			break;
		default:
			printk("Invalid argument");
			return -EINVAL;
	}

	return 0;
}

//物理地址转虚拟地址
int fs4412_bee_ioremap(void)
{
	int ret;

	gpx1con = ioremap(FS4412_GPX1CON, 4);
	if (gpx1con == NULL) {
		printk("ioremap gpx1con\n");
		ret = -ENOMEM;
		return ret;
	}

	gpx1dat = ioremap(FS4412_GPX1DAT, 4);
	if (gpx1dat == NULL) {
		printk("ioremap gpx1dat\n");
		ret = -ENOMEM;
		return ret;
	}

	return 0;
}

void fs4412_bee_iounmap(void)
{
	iounmap(gpx1con);
	iounmap(gpx1dat);
}

void fs4412_bee_io_init(void)
{

	writel(readl(gpx1con) & ~(0xf<<20) | 0x1 << 20, gpx1con);
	writel(readl(gpx1dat) & ~(0x1<<5), gpx1dat);


}
	
struct file_operations bee_fops = {
	.owner = THIS_MODULE,
	.open = bee_open,
	.release = bee_release,
	.unlocked_ioctl = bee_unlocked_ioctl,
};

static int bee_init(void)
{
	dev_t devno = MKDEV(BEE_MA, BEE_MI);
	int ret;

	ret = register_chrdev_region(devno,BEE_NUM, "bee");
	if (ret < 0) {
		printk("register_chrdev_region\n");
		return ret;
	}

	cdev_init(&cdev, &bee_fops);
	cdev.owner = THIS_MODULE;
	ret = cdev_add(&cdev, devno,BEE_NUM);
	if (ret < 0) {
		printk("cdev_add\n");
		goto err1;
	}

	ret = fs4412_bee_ioremap();
	if (ret < 0)
		goto err2;


	fs4412_bee_io_init();
    struct class *cls;
    cls = class_create(THIS_MODULE, "bee");
    device_create(cls, NULL, devno, NULL, "bee");

	printk("bee init\n");

	return 0;
err2:
	cdev_del(&cdev);
err1:
	unregister_chrdev_region(devno, BEE_NUM);
	return ret;
}

static void bee_exit(void)
{
	dev_t devno = MKDEV(BEE_MA, BEE_MI);

	fs4412_bee_iounmap();
	cdev_del(&cdev);
	unregister_chrdev_region(devno, BEE_NUM);
	printk("bee exit\n");
}

module_init(bee_init);
module_exit(bee_exit);
