#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h> //追加
#include <linux/uaccess.h> //ヘッダに追加
#include <linux/io.h> //ヘッダファイルを追加

MODULE_AUTHOR("Haruto Itami");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL; //追加
static volatile u32 *gpio_base = NULL; //アドレスをマッピングするための配列をグローバルで定める。

static ssize_t led_write(struct file* flip, const char* buf, size_t count, loff_t* pos){
	char c;
	int i;
	if(copy_from_user(&c,buf,sizeof(char)))
		return -EFAULT;

//	printk(KERN_INFO "receive %c\n", c);

	if(c == '0')
		gpio_base[10] = 1 << 25; //LEDを消灯する。

	else if (c == '1')
		gpio_base[7] = 1 << 25; //LEDを点灯する。

	return 1; //読み込んだ文字数を返す（この場合はダミーの１）
}

/* 講義内で作ったsushiプログラム 
 * static ssize_t  sushi_reed(struct file* flip, char* buf, size_t count, loff_t* pos) {
	int size = 0;
	char sushi[] = {'s', 'u', 's', 'h', 'i',  0x0A};
	if (copy_to_user(buf+size, (const char *)sushi, sizeof(sushi))){
		printk(KERN_ERR "sushi : copy_to_user failed\n");
		return -EFAULT;
	}
	size += sizeof(sushi);
	return size;
}*/

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
//	.read = sushi_reed //sushiプログラム起動用
};


static int __init init_mod(void){
	int retval;
	retval = alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
		printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;
	}

	printk(KERN_INFO "%s is loaded. major:%d\n", __FILE__,MAJOR(dev));

	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev, 1);
	if(retval < 0){
		printk(KERN_ERR "cdev_add failed. major:%d, minor*%d", MAJOR(dev), MINOR(dev));
		return retval;
	}

	cls = class_create(THIS_MODULE, "myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.");
		return PTR_ERR(cls);
	}

	device_create(cls, NULL, dev, NULL, "myled%d", MINOR(dev));
	gpio_base = ioremap_nocache(0xfe200000, 0xA0); 
	
	const u32 led = 25;
	const u32 index = led/10; //GPFSEL2
	const u32 shift = (led%10)*3; //15bit
	const u32 mask = ~(0x7 << shift);
	gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift); // 001: output flag
	
	return 0;
}

static void __exit cleanup_mod(void){

	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls); //追加
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__, MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);
