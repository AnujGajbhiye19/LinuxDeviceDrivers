#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xc2b78c96, "module_layout" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xb65e5a32, "class_destroy" },
	{ 0x22b90774, "cdev_del" },
	{ 0xbd9cb3ec, "device_destroy" },
	{ 0x17c294af, "device_create" },
	{ 0xc4952f09, "cdev_add" },
	{ 0x2064fa56, "cdev_init" },
	{ 0x2871e975, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x56470118, "__warn_printk" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "70E92406E07449AF10A5E06");
