#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h> //for ssize_t
#include <linux/cdev.h>

#define DEVICE_NAME "chardev_98"
#define BUF_LEN 80

char Message[BUF_LEN] = "Default String" , *msg_ptr;

static dev_t devno=0;
static struct cdev mycdev;

static int major = 230;
static bool write_buff_flag = false;
static int counter = 0;


//Executed when process attempted to read from a device
ssize_t joseph98_dev_read (struct file *fs, char __user *buffer, size_t size, loff_t *offset)
{

    printk("dev_read\n");
    int byte_read = 0;
    if (*msg_ptr == 0) return 0;

    while(size && *msg_ptr) {
        put_user(*(msg_ptr++), buffer++);
        size--;
        byte_read++;
    }
    return byte_read;
}

//Executed when process attempted to write to a device
ssize_t joseph98_dev_write (struct file *fs, char __user *buffer, size_t size, loff_t *lo)
{
    printk("<1>dev_write: size =%d\n", size);
    write_buff_flag = true;
    int i;
    for (i = 0; i < size && i < BUF_LEN; i++) {
        get_user(Message[i], buffer + i);
    }
    msg_ptr = Message;
    *(msg_ptr + i) = NULL;
    printk("kernel space: userspace write [%s] to kernel space\n", msg_ptr);
    return i;
}

//Executed at every open time
int joseph98_dev_open(struct inode *inode, struct file *fs)
{
    printk("<1>dev_open\n");
    //+++
    if (!write_buff_flag){
       sprintf(Message, "Open %d times\n", counter++);
       msg_ptr = Message;
    } else {
       write_buff_flag = false;
    }
    //---
    return 0;
}

//Executed at every close time
int joseph98_dev_release(struct inode *inode, struct file *fs)
{
    printk("<1>dev_release\n");
    return 0;
}

static struct file_operations fops = {
    .read=joseph98_dev_read,
    .write=joseph98_dev_write,
    .open=joseph98_dev_open,
    .release=joseph98_dev_release
};

static int __init joseph98_hello_init(void)
{

    // Register the device
    register_chrdev_region(MKDEV(major, 0), 1, DEVICE_NAME);
    cdev_init(&mycdev, &fops);  //Link with file_operations structure
    cdev_add(&mycdev, MKDEV(major, 0), 1);
    return 0;
}

static void __exit joseph98_hello_exit(void)
{
    printk(KERN_DEBUG "exit device driver\n");

    //unregister device
    cdev_del(&mycdev);
    unregister_chrdev_region(MKDEV(major, 0), 1);

}

module_init(joseph98_hello_init);
module_exit(joseph98_hello_exit);

