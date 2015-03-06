#include <linux/module.h>
//#include <linux/kprobes.h>
//#include <linux/kallsyms.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>

typedef struct _MY_DEV_DATA_{
	char* name;
	int age;
}MY_DEV_DATA;

static MY_DEV_DATA my_prv_data={
	.name= "hello iskey",
	.age= 17,
};

static void my_dev_release(struct device *dev)
{
	return;
}

static struct platform_device my_device ={
	.name = "iskey",
	.id = 0,
	.dev = {
		.platform_data= &my_prv_data,
		.release= my_dev_release,
	},
	.num_resources = 0,
};

static int my_dev_probe(struct platform_device *pdev)
{
	printk("hello, I am be probed.\n");
	MY_DEV_DATA *t= pdev->dev.platform_data;
	printk("my private data is %s.\n", t->name);
	return 0;
}

static int my_dev_remove(struct platform_device *pdev)
{
	printk("device will be removed.\n");
	return 0;
}

static struct platform_driver my_device_driver={
	.probe= my_dev_probe,
	.remove= my_dev_remove,
	.driver={
		.name= "iskey",
	},
};

static void __init my_platform_init(void)
{
	platform_driver_register(&my_device_driver);
	platform_device_register(&my_device);
} 


static void __exit my_platform_cleanup(void)
{
	printk("I will exited.\n");
	platform_driver_unregister(&my_device_driver);
	platform_device_unregister(&my_device);
}

module_init(my_platform_init);
module_exit(my_platform_cleanup);
MODULE_LICENSE("GPL");

