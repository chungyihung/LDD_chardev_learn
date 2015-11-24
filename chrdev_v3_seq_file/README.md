======================Testing============================   
1. make  
2. sudo make install  
   (module will be cp to /lib/modules/3.8.0-44-generic/kernel/drivers/char)  
3. sudo modprobe chardev  
4. sudo mknod ./hello --mode=666 c 230 0  
  
Try to see that:  
1. proc we create by seq_file method  
   cat /proc/test98  
2. write and read string to/from chardev  
   echo "string you want" > ./hello  
   cat ./hello  
  
===================Clean and rmmod======================  
1. make clean  
2. sudo make uninstall  
3. sudo modprobe -r chardev  
4. rm hello  
