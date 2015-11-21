#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>    //for ssize_t
#include <linux/cdev.h>
#include <linux/proc_fs.h>  //for /proc filesystem
#include <linux/seq_file.h>

#define DEVICE_NAME "chardev_98"
#define BUF_LEN 80
#define DEVICE_PROC_NAME "test98"
static char *sampleArray[3]=
            {"string1",
            "string2",
            "string3"};

char Message[BUF_LEN] = "Default String" , *msg_ptr;

static dev_t devno=0;
static struct cdev mycdev;
struct proc_dir_entry *Proc_File_98;

static int major = 230;
static bool write_buff_flag = false;
static int counter = 0;

/* Using seq_file interface */

static void *j98_seq_start(struct seq_file *sfile, loff_t *pos) {
    printk("In start: *pos = %llu\n", *pos);
    if (*pos >= 3)
        return NULL;
    else
        return (void *)((unsigned long) *pos + 1);
}

static void *j98_seq_next(struct seq_file *sfile, void *v, loff_t *pos) {
    ++*pos;
    printk("In next: *pos = %llu\n", *pos);
    return j98_seq_start(sfile, pos);
}

static void *j98_seq_stop(struct seq_file *sfile, void *v) {
    //In this case we have no item to be clean..
}

static int *j98_seq_show(struct seq_file *s, void *v) {
    printk("In seq_show function\n");
    int n = (int) v - 1;
    //Write information to seq_file via seq_printf
    seq_printf(s, "sampleArray[%d]: %s \n", n, sampleArray[n]);
    return 0;
}

static struct seq_operations seq_test_node = {
    .start  =   j98_seq_start,
    .next   =   j98_seq_next,
    .stop   =   j98_seq_stop,
    .show   =   j98_seq_show
};

//Create file_operations structure
static int seq_proc_open(struct inode *inode, struct file *file) {
    return seq_open(file, &seq_test_node);
}

static struct file_operations seq_proc_ops = {
    .owner  = THIS_MODULE,
    .open   = seq_proc_open,    //Only open function we need to implement
    .read   = seq_read,
    .llseek = seq_lseek,
    .release= seq_release
};

/* /proc debugging function */

ssize_t procfile_read(char *buffer, char ** buffer_location, off_t offset,
        int buffer_len, int *eof, void *data)
{
    int len = 0;
    static int count = 0;
    if (offset > 0) {
        len = 0;
    }
    else {
        /* This is the most important place!
         * The first argument of procfile_read(read_proc_t) is buffer allocated by kernel.
         * We only need to write information to the buffer, and kernel copy_to_user space
         * from buffer automatically.*/
        len = sprintf(buffer, "For the %d time, go away!\n", count++);
    }
    return len;
}

//Executed when process attempted to read from a device
ssize_t dev_read (struct file *fs, char __user *buffer, size_t size, loff_t *offset)
{

    printk("<1>dev_read\n");
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
ssize_t dev_write(struct file *fs, char __user *buffer, size_t size, loff_t *lo)
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
int dev_open(struct inode *inode, struct file *fs)
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

#undef PDEBUG
#ifdef MYDEBUG      //Use MYDEBUG FLAG as a switcher
    #define PDEBUG(fmt, args...) printk(KERN_DEBUG "chardev_98: " fmt, ## args)
#else
    #define PDEBUG(fmt, args...)
#endif

static int __init hello_init(void)
{
    // Register the device
    register_chrdev_region(MKDEV(major, 0), 1, DEVICE_NAME);
    cdev_init(&mycdev, &fops);
    cdev_add(&mycdev, MKDEV(major, 0), 1);

    //Specified the file_operations structure!
    Proc_File_98 = create_proc_entry(DEVICE_PROC_NAME, 0444, NULL);
    Proc_File_98->proc_fops = &seq_proc_ops;

    return 0;
}

static void __exit hello_exit(void)
{
    printk("<1>exit device driver\n");
    remove_proc_entry(DEVICE_PROC_NAME, NULL);
    //unregister device
#if 0
    unregister_chrdev(major, "char_reg");
#else
    cdev_del(&mycdev);
    unregister_chrdev_region(MKDEV(major, 0), 1);
#endif

}

module_init(hello_init);
module_exit(hello_exit);

