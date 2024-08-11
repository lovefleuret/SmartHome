#ifndef __UARTTOOL_H

#define __UARTTOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "wiringSerial.h"

#define SERIAL_DEV "/dev/ttyS5"
#define BAUD 115200


int myserialOpen (const char *device, const int baud);
void serialSendstring (const int fd, const unsigned char *s, int len);
int serialGetstring (const int fd, unsigned char *buffer, const int bytesLen);





#endif


