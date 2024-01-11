/*Include section*/

#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>

/*Global declaration and Macro section */

#define DEV_MEM_SIZE 512
#undef pr_fmt
#define pr_fmt(fmt) "(%d:%d): %s: " fmt,MAJOR(dev_n),MINOR(dev_n),__func__

char dev_buff[DEV_MEM_SIZE];
dev_t dev_n;
struct cdev pcd_cdev;
struct class *class_pcd;
struct device *device_pcd;

/*Method Definitions */

loff_t pcd_lseek(struct file *filp, loff_t offset, int whence){
	
	loff_t temp;
	
	pr_info("lseek requested\n");
	pr_info("Current File Position: %lld\n",filp->f_pos);
	
	switch(whence){
		case SEEK_SET:
			if((offset > DEV_MEM_SIZE) || (offset < 0))
				return -EINVAL;
			filp->f_pos = offset;
			break;
		case SEEK_CUR:
			temp = filp->f_pos + offset;
			if((temp > DEV_MEM_SIZE) || (temp < 0))
				return -EINVAL;
			filp->f_pos = temp;
			break;
		case SEEK_END:
			temp = DEV_MEM_SIZE + offset;
			if(temp > DEV_MEM_SIZE || temp < 0)
				return -EINVAL;
			filp->f_pos = temp;
			break;
		default:
			return -EINVAL;
	}
	pr_info("Updated File Position:%lld\n",filp->f_pos);
	
	return filp->f_pos;
}
ssize_t pcd_read(struct file *filp, char __user *buff, size_t count, loff_t *f_pos){
	
	pr_info("Read requested for %zu bytes\n",count);
	pr_info("Current File Position: %lld\n",*f_pos);
	
	if((count + *f_pos) > DEV_MEM_SIZE){
		count = DEV_MEM_SIZE - *f_pos;
	}
	
	if(copy_to_user(buff, &dev_buff[*f_pos], count)){
	    return -EFAULT;
	}
	
	*f_pos += count;
	
	pr_info("Number of Bytes successfully Read: %zu\n",count);
	pr_info("Updated File Position:%lld\n",*f_pos);
	
	return count;
}
ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos){
	
	pr_info("Write requested for %zu bytes\n",count);
	pr_info("Current File Position: %lld\n",*f_pos);
	
	if((count + *f_pos) > DEV_MEM_SIZE){
		count = DEV_MEM_SIZE - *f_pos;
	}
	
	if(!count){
		pr_err("No space left on the device \n");
		return -ENOMEM;
	}
        
	if(copy_from_user(&dev_buff[*f_pos], buff, count)){
		return -EFAULT;
	}
	
	*f_pos += count;
	
	pr_info("Number of Bytes successfully Written: %zu\n",count);
	pr_info("Updated File Position:%lld\n",*f_pos);
	
	return count;
}
int pcd_open(struct inode *inode, struct file *filp){
	
	pr_info("Open Was Successfull\n");
	
	return 0;
}
int pcd_release(struct inode *inode, struct file *filp){
	
	pr_info("Release was Successfull\n");
	
	return 0;
}

/*File Operations Section */

struct file_operations pcd_fops = {
	.open = pcd_open,
	.release = pcd_release,
	.read = pcd_read,
	.write = pcd_write,
	.owner = THIS_MODULE
};

/*Init and cleanup Definitions*/

static int __init pcd_driver_init(void){
	//1) generate device number dynamically
	int res = alloc_chrdev_region(&dev_n, 0, 1, "npcd_devices");
	
	if(res<0){
		pr_err("Alloc chrdev failed\n");
		goto out;
	}
	pr_info("Device Number <MAJOR>:<MINOR> = %d:%d \n",MAJOR(dev_n),MINOR(dev_n));
	
	//initialize cdev structure
	cdev_init(&pcd_cdev, &pcd_fops);
	
	//register cdev structure (device) with vfs
	pcd_cdev.owner = THIS_MODULE;
	res = cdev_add(&pcd_cdev, dev_n, 1);
	if(res < 0){
    		pr_err("Cdev add failed\n");
    		goto unreg_chrdev;
	}
	
	//create dev class under /sys/class/
	class_pcd = class_create(THIS_MODULE,"npcd_class");
	if(IS_ERR(class_pcd)){
		pr_err("Class creation failed\n");
		res = PTR_ERR(class_pcd);
		goto cdev_del;
	}
	
	//create device and populate its information on sysfs
	device_pcd = device_create(class_pcd, NULL, dev_n, NULL, "pcd");
	if(IS_ERR(device_pcd)){
		pr_err("Device create failed\n");
		res = PTR_ERR(device_pcd);
		goto class_del;
	}
	
	pr_info("Module Initialized\n");
	return 0;
	
class_del:
        class_destroy(class_pcd);
cdev_del:
        cdev_del(&pcd_cdev);
unreg_chrdev:
        unregister_chrdev_region(dev_n,1);
out:
        pr_info("Module insertion failed\n");
        return res;
}

static void __exit pcd_driver_exit(void){
	device_destroy(class_pcd, dev_n);
	class_destroy(class_pcd);
	cdev_del(&pcd_cdev);
	unregister_chrdev_region(dev_n, 1);
	
	pr_info("Module Unloaded\n");
		
}

/*Module Registration section*/

module_init(pcd_driver_init);
module_exit(pcd_driver_exit);

/*Module description section*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NOBODY.ko");
MODULE_DESCRIPTION("This is a practice pseudo char driver");
