#include <fcntl.h> // for open()
#include <unistd.h> // for exit()
#include <sys/ioctl.h> //ioctl
#include <stdio.h>
static char *dev="/dev/hello", message[100];

void main()
{
    int file_desc;
    // open the device file, ex: /dev/hello
    file_desc = open(dev, O_RDWR);
    if( file_desc < 0) {
        printf("Failed to open file\n");
        exit(-1);
    }

    //Read from the device file
    if(read(file_desc, message, sizeof(message)) < 0 ) {
        printf("Failed to read the file\n");
        exit(-1);
    }

    printf("get_msg: %s\n", message);

    close(file_desc);
}
