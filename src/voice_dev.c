#include "input_dev.h"
#include "global.h"
#include <stdio.h>
#include <pthread.h>
#include "uartTool.h"
#include "msg_queue.h"
#include "voice_dev.h"
#include <stdio.h>

#define VOICE_DEV_NAME "/dev/ttyS5"
#define BAUD_RATE 115200

int serial_fd = -1;

static int voice_dev_init(void)
{
    serial_fd = serialOpen(VOICE_DEV_NAME, BAUD_RATE);
    if (serial_fd < 0)
    {
        printf("open serial failed\n");
        return -1;
    }
    return serial_fd;
}

static void voice_dev_final(void)
{
    if (serial_fd > 0)
    {
        serialClose(serial_fd);
    }
    return;
}

static void *voice_get(void *arg) //! 发送
{
    unsigned char buffer[6] = {0};  

    globalValue_t *gVioce = (globalValue_t *)arg;

    if (gVioce == NULL)
    {
        printf("voice_dev arg is NULL\n");
        pthread_exit(NULL);
    }
    if (gVioce->mqd == -1)
    {
        printf("voice_dev mqd is -1\n");
        pthread_exit(NULL);
    }

    int bytesRecv = -1;
    while (1)
    {
        bytesRecv = serialGetstring(serial_fd, buffer , 6);
        if (bytesRecv == -1)
        {
            perror("voice_dev serial recv failed   \n ");
            printf("%s --- %d", __func__, __LINE__);
        }
        else if (bytesRecv > 0 && buffer[0] == 0xAA && buffer[1] == 0x55 && buffer[4] == 0x55 && buffer[5] == 0xAA)
        {
            msg_queue_send(gVioce->mqd, buffer, 6);
        }
    }
    pthread_exit(NULL);
}
//! 语音播报
static void *voice_set(void *arg)
{
    unsigned char *buffer = (unsigned char *)arg;
    pthread_detach(pthread_self());
    if( -1 == serial_fd) //! serial_fd 为全局变量
    {
        serial_fd = voice_dev_init();
        if( -1 == serial_fd)
        {
            printf("voice_dev_init failed\n");
            pthread_exit(NULL);
        }
    }

    if(NULL != buffer)
    {   
        printf("buffer[0]:%x buffer[1]:%x buffer[2]:%x buffer[3]:%x buffer[4]:%x buffer[5]:%x\n",
                 buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
        printf("------%s-------%d\n", __func__, __LINE__);
        serialSendstring(serial_fd, buffer, 6); //! MCU发送给语音模块
    }
    pthread_exit(NULL);
}

input_dev_t voice_dev = {
    .input_dev_name = "voice_dev",
    .init = voice_dev_init,
    .final = voice_dev_final,
    .recvRoutine = voice_get,
    .sendRoutine = voice_set,
    .next = NULL,
};

input_dev_t *add_voice_dev(input_dev_t *phead)
{
    return add_input_dev(phead, &voice_dev);
}