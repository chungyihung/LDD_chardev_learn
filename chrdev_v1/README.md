This test fix major number to be 230 and device name as joseph98
1. make
2. sudo insmod chardev.ko
3. sudo mknod /dev/joseph98 c 230 0
   (cat /proc/devices |grep 230 you would see the device)
   /dev/[any_name_you_want]
4. cat /dev/joseph98
   echo 3>/dev/joseph98 (Here we do not print anything to stdout)
   you will see printk message in dmesg
5. rm /dev/joseph98
   rmmod chardev
