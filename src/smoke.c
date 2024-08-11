#include "smoke.h"

#define SMOKE_PIN 7
#define SMOKE_MODE INPUT
static int smoke_init()
{
    pinMode(SMOKE_PIN, SMOKE_MODE);
}

static void smoke_close()
{
    //do nothing
    return;
}

//! 煤气浓度检测线程
static void *smoke_get(void *arg)
{
    globalValue_t *pValue = (globalValue_t *)arg;

    mqd_t mqd = pValue->mqd;

    if (pValue == NULL)
    {
        perror("smoke_get pValue error");
        pthread_exit(NULL);
    }
    if (mqd == -1)
    {
        perror("smoke_get mq_open error");
        pthread_exit(NULL);
    }

    unsigned char buffer[6] = {0xAA, 0x55, 0x00, 0x00, 0x55, 0xAA};
    int smoke = HIGH;
    while (1)
    {
        smoke = digitalRead(SMOKE_PIN); //! 读取煤气浓度
        if (smoke == LOW)
        {
            printf("煤气浓度过高！\n");
            buffer[2] = 0x00;
            buffer[3] = 0x01;
            msg_queue_send(mqd, buffer, 6); 
        }
        else
        {
            buffer[2] = 0x00;
            buffer[3] = 0x00;
            msg_queue_send(mqd, buffer, 6);
        }
        
        sleep(6);
    }
}

static void *smoke_send(void *arg)
{
    // do nothing
}

input_dev_t smoke_dev = {
    .input_dev_name = "smoke_dev",
    .init = smoke_init,
    .final = smoke_close,
    .recvRoutine = smoke_get,
    .sendRoutine = smoke_send,
    .next = NULL};

input_dev_t *add_smoke_dev(input_dev_t *head)
{
    return add_input_dev(head, &smoke_dev);
}