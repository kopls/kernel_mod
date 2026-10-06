#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "inc/my_ioctl.h"

int main(void)
{
    int fd;
    unsigned long size;

    fd = open("/dev/my_device", O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return EXIT_FAILURE;
    }

    if (ioctl(fd, MY_IOCTL_GET_SIZE, &size) < 0)
    {
        perror("ioctl GET_SIZE");
        close(fd);
        return EXIT_FAILURE;
    }

    if (ioctl(fd, MY_IOCTL_CLEAR) < 0)
    {
        perror("ioctl CLEAR");
        close(fd);
        return EXIT_FAILURE;
    }

    if (ioctl(fd, MY_IOCTL_GET_SIZE, &size) < 0)
    {
        perror("ioctl GET_SIZE");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);

    return EXIT_SUCCESS;
}
