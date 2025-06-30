#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_FILENAME "/dev/ledkey"

int main(int argc, char *argv[])
{
    int dev;
    char buff, key;
    int ret;
	int val, key_data, key_data_old = 0;

	if(argc < 2)
    {
        printf("USAGE : %s ledval[0x00 ~ 0xff]\n", argv[0]);
        return 1;
    }

    printf("1) device file open\n");

   if(access(DEVICE_FILENAME, F_OK) != 0)
    {   
        int ret = mknod(DEVICE_FILENAME, S_IRWXU | S_IRWXG | S_IFCHR, (230 << 8) | 1);
        if(ret < 0)
            perror("mknod()");
    }  

    dev = open(DEVICE_FILENAME, O_RDWR | O_NDELAY);
	if(dev < 0)
	{
		perror("open()");
		return 1;
	} 
	key = strtoul(argv[1], NULL, 16);
	ret = write(dev, &key, sizeof(key));

	do {
        usleep(100000);  //100MSec
		ret = read(dev, &buff, sizeof(buff));
//		ret = read(dev, &key_data, sizeof(key_data));
		if(ret < 0)
		{
			perror("read()");
			return ret;
		}
		key_data = (unsigned char)buff;
        if(key_data != key_data_old)
        {
        	key_data_old = key_data;
            if(key_data)
            {
            	val = key_data;
                puts("0:1:2:3:4:5:6:7");
				for(int i=0;i<8;i++)
                {
               //if(key_data & (0x01 << i))
				if((key_data >> i) & 0x01)
                	putchar('O');
				else
					putchar('X');
				if(i != 7 )
					putchar(':');
				else
					putchar('\n');
				}
				putchar('\n');
//    			ret = write(dev, &key_data, sizeof(key_data));
    			ret = write(dev, &buff, sizeof(buff));
				if(ret < 0)
				{
					perror("write");
					return ret;
				}
			}
			if(key_data == 0x80)
				break;
		}
	}while(1);

    ret = close(dev);
    return 0;
}
