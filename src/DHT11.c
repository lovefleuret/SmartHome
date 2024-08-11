#include "DHT11.h"
typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;

#define HIGH_TIME 32
#define DHT11PIN 2
#define MAX_WAIT_LOOP 50000
uint32 dataBuffer;

static int check_response(void)
{
    int loopCount = 0;
    while (digitalRead(DHT11PIN))
        if (loopCount++ == MAX_WAIT_LOOP)
            return -1;
    loopCount = 0;
    while (!digitalRead(DHT11PIN))
        if (loopCount++ == MAX_WAIT_LOOP)
            return -1;
    return 0;
}

static int readSensorData(void)
{
    uint8 crc;
    uint8 i;

    pinMode(DHT11PIN, OUTPUT);
    digitalWrite(DHT11PIN, 0);
    delay(25);
    digitalWrite(DHT11PIN, 1);
    pinMode(DHT11PIN, INPUT);
    pullUpDnControl(DHT11PIN, PUD_UP);

    if (check_response() == -1)
        return -1;

    for (i = 0; i < 32; i++)
    {
        if (check_response() == -1)
            return -1;

        delayMicroseconds(HIGH_TIME);
        dataBuffer *= 2;
        if (digitalRead(DHT11PIN) == 1)
            dataBuffer++;
    }

    for (i = 0; i < 8; i++)
    {
        if (check_response() == -1)
            return -1;

        delayMicroseconds(HIGH_TIME);
        crc *= 2;
        if (digitalRead(DHT11PIN) == 1)
            crc++;
    }

    return 0;
}

static int DHT11Init(void)
{
    pinMode(DHT11PIN, OUTPUT);
    digitalWrite(DHT11PIN, 1);

    delayMicroseconds(27);
    return 0;
}

void DHT11Final(void)
{
    // do nothing
    return;
}

void *DHT11Read(void *arg)
{
    char buf_DHT11[128];
    globalValue_t *pDHT11 = (globalValue_t *)arg;
    while (1)
    {
        if (readSensorData() == 0)
        {
            // Assume the read was successful
            printf("Sensor data read success !\n");
            printf("RH:%ld.%ld\n", (dataBuffer >> 24) & 0xff, (dataBuffer >> 16) & 0xff);
            printf("TMP:%ld.%ld\n", (dataBuffer >> 8) & 0xff, dataBuffer & 0xff);
            // sprintf(buf_RH, "Humidity: %ld.%01ld", (dataBuffer >> 24) & 0xff, (dataBuffer >> 16) & 0xff % 10);
            // sprintf(buf_TMP, "Temperature: %ld.%01ld", (dataBuffer >> 8) & 0xff, dataBuffer & 0xff % 10);

            int TMP_f = dataBuffer & 0xff;
            while (TMP_f >= 10)
            {
                TMP_f /= 10; //! 温度小数部分只保留一位小数
            }

            int TMP_i = (dataBuffer >> 8) & 0xff;
            while (TMP_i >= 100)
            {
                TMP_i /= 10; //! 整数保留两位
            }
            int RH_i = (dataBuffer >> 24) & 0xff;
            while (RH_i > 100)
            {
                RH_i /= 10; //! 湿度整数部分只保留两位，相对湿度一般不能超过100%
            }
            int RH_f = (dataBuffer >> 16) & 0xff;
            while (RH_f >= 10)
            {
                RH_f /= 10; //! 温度小数部分只保留一位小数
            }
            if (RH_i < 10 && TMP_i < 10)
            {
                sprintf(buf_DHT11, "RH: 0%d.%d  TMP: 0%d.%d", RH_i, RH_f, TMP_i, TMP_f);
            }
            else
            {
                sprintf(buf_DHT11, "RH: %d.%d  TMP: %d.%d", RH_i, RH_f, TMP_i, TMP_f);
            }

            msg_queue_send(pDHT11->mqd, buf_DHT11, strlen(buf_DHT11) + 1);
            // oled_putstrto(&disp, 0, 18 + 2, buf_RH);
            // oled_putstrto(&disp, 0, 27 + 3, buf_TMP);
            // oled_send_buffer(&disp);
        }
        else
        {
            printf("Sensor does not respond!\n");
            DHT11Init();
            delay(2000);
            continue;
        }
        memset(buf_DHT11, 0, sizeof(buf_DHT11));
        dataBuffer = 0;
        delay(2000);
    }

    pthread_exit(NULL);
}

void *DHT11Send(void *arg)
{
    // do nothing
    return NULL;
}

input_dev_t DHT1_dev = {
    .input_dev_name = "DHT11",
    .init = DHT11Init,
    .recvRoutine = DHT11Read,
    .sendRoutine = DHT11Send,
    .final = DHT11Final,
    .next = NULL};

input_dev_t *add_DHT1_dev(input_dev_t *phead)
{
    return add_input_dev(phead, &DHT1_dev);
}
