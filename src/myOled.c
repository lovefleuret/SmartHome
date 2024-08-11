#include "myOled.h"

#define IICFILE "/dev/i2c-3"
struct display_info disp;

int myOledInit()
{
    disp.address = 0x3c;
    disp.font = font2;
    int ret = -1;
    ret = oled_open(&disp, IICFILE);
    if (ret < 0)
    {
        perror("oled_open failed!\n"); //perror 不用在缓冲区中输出，直接输出到标准错误
        return -1;
    }
    ret = oled_init(&disp);
    if (ret < 0)
    {
        perror("oled_init failed!\n");
        return -1;
    }
    return 0;
}

int myOledClose()
{
    oled_close(&disp);
    return 0;
}

