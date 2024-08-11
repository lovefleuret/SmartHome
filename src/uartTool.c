#include "uartTool.h"

int myserialOpen(const char *device, const int baud)
{
    struct termios options;
    speed_t myBaud;
    int status, fd;
    switch (baud)
    {
    case 9600:
        myBaud = B9600;
        break;
    case 115200:
        myBaud = B115200;
        break;
    }

    
    if ((fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
    {   
        perror("open error");
        return -1;
    }
        
    fcntl(fd, F_SETFL, O_RDWR);  // Configure port reading
    // Get and modify current options:
    tcgetattr(fd, &options);
    cfmakeraw(&options);
    cfsetispeed(&options, myBaud);
    cfsetospeed(&options, myBaud);

    
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 100; // Ten seconds (100 deciseconds)

    

    tcsetattr(fd, TCSANOW, &options);
    ioctl(fd, TIOCMGET, &status);
    status |= TIOCM_DTR;
    status |= TIOCM_RTS;
    ioctl(fd, TIOCMSET, &status);
    usleep(10000); // 10mS


    return fd;
}


void serialSendstring(const int fd, const unsigned char *s, int len)
{
    int ret;
    ret = write(fd, s, len);
    
    if (ret < 0)
    {
        printf("Serial Puts Error\n");
        exit(-1) ;
    }
    return;
}

int serialGetstring(const int fd, unsigned char *buffer, int bytesLen)
{   
    int n_read;
    
    n_read = read(fd, buffer, bytesLen);
    

    if(-1 == n_read)
    {
        perror("read");
        exit(-1);
    }
    
    return n_read;
    
}




