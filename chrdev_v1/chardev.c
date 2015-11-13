#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h> //for ssize_t
#include <linux/cdev.h>
static dev_t devno=0;
static struct cdev mycdev;

static int major = 230;
#define DEV_NAME "joseph98"
#define BUF_LEN 80
char Message[BUF_LEN] = "Default String" , *msg_ptr;

ssize_t dev_read (struct file *fs, char __user *buffer, size_t size, loff_t *lo)
{
    printk("<1>dev_read\n");
    return 0;
}

ssize_t dev_write (struct file *fs, char __user *buffer, size_t size, loff_t *lo)
{
    printk("<1>dev_write\n");
    int i;
    for (i = 0; i < size && i < BUF_LEN; i++) {
        get_user(Message[i], buffer + i);
    }
    msg_ptr = Message;
    return i;
}

int dev_open(struct inode *inode, struct file *fs)
{
    printk("<1>dev_open\n");
    return 0;
}

int dev_release(struct inode *inode, struct file *fs)
{
    printk("<1>dev_release\n");
    return 0;
}

static struct file_operations fops = {
    .read=dev_read,
    .write=dev_write,
    .open=dev_open,
    .release=dev_release
};

static int __init hello_init(void)
{
#if 0
    major = register_chrdev(0,"char_reg", &fops);
    printk("<1>init device driver\n");
    if (major < 0) {
        printk ("<1> Register char device fail\n");
        return -1;
    }
#else
    register_chrdev_region(MKDEV(major, 0), 1, DEV_NAME);
    cdev_init(&mycdev, &fops);
    cdev_add(&mycdev, MKDEV(major, 0), 1);
#endif
    return 0;
}

static void __exit hello_exit(void)
{
    printk("<1>exit device driver\n");
#if 0
    unregister_chrdev(major, "char_reg");
#else
    cdev_del(&mycdev);
    unregister_chrdev_region(MKDEV(major, 0), 1);
#endif
}

module_init(hello_init);
module_exit(hello_exit);

