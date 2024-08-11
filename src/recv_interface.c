#include "recv_interface.h"

static output_dev_t *ohead = NULL;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

static int handler(void *user, const char *section, const char *name,
                   const char *value) //! 回调函数，处理响应事件
{
    struct output_dev *tempDev = NULL;
    if (NULL == ohead)
    {
        ohead = (output_dev_t *)malloc(sizeof(output_dev_t));
        memset(ohead, 0, sizeof(output_dev_t));
        strcpy(ohead->output_dev_name, section); //! section 为设备名
    }
    else if (strcmp(ohead->output_dev_name, section))
    {
        tempDev = (output_dev_t *)malloc(sizeof(output_dev_t));
        memset(tempDev, 0, sizeof(output_dev_t));
        strcpy(tempDev->output_dev_name, section); //! section 为设备名
        tempDev->next = ohead;
        ohead = tempDev; //! 不带头节点的头插法
    }

    if (NULL != ohead)
    {
        if (MATCH(ohead->output_dev_name, "key"))
        {
            sscanf(value, "%x", &ohead->key);
        }
        else if (MATCH(ohead->output_dev_name, "gpio_pin"))
        {
            ohead->gpio_pin = atoi(value);
        }
        else if (MATCH(ohead->output_dev_name, "gpio_mode"))
        {
            if (!strcmp(value, "OUTPUT")) //! C语言中字符串不能直接比较，需要使用strcmp函数
                ohead->gpio_mode = OUTPUT;
            if (!strcmp(value, "INPUT"))
                ohead->gpio_mode = INPUT;
        }
        else if (MATCH(ohead->output_dev_name, "gpio_value"))
        {
            if (!strcmp(value, "HIGH"))
                ohead->gpio_value = HIGH;
            if (!strcmp(value, "LOW"))
                ohead->gpio_value = LOW;
        }
        else if (MATCH(ohead->output_dev_name, "check_face_status"))
        {
            ohead->check_face_status = atoi(value);
        }
        else if (MATCH(ohead->output_dev_name, "voice_set_status"))
        {
            ohead->voice_set_status = atoi(value);
        }
    }
}

static int recv_interface_init(void)
{

    if (ini_parse("./output_dev.ini", handler, NULL) < 0)
    {
        printf("Can't load 'output_dev.ini'\n");
        return -1;
    }
#if DEBUG
    output_dev_t *p = ohead;
    while (p != NULL)
    {
        printf("output_dev_name = %s\n", p->output_dev_name);
        printf("key = %#x\n", p->key);
        printf("gpio_pin = %d\n", p->gpio_pin);
        printf("gpio_mode = %d\n", p->gpio_mode);
        printf("gpio_value = %d\n", p->gpio_value);
        printf("check_face_status = %d\n", p->check_face_status);
        printf("voice_set_status = %d\n", p->voice_set_status);
        p = p->next;
    }
#endif
    myOledInit();
    face_init();
    return 0;
}

static void recv_interface_final(void)
{
    mq_unlink("/test"); //! 释放资源
    myOledClose();
    face_final();
    return;
}

static void *msg_handle(void *arg)
{
    input_dev_t *p = NULL;
    int ret = -1;
    pthread_detach(pthread_self()); // 设置线程与主线程分离
    recv_msg_t *recv_msg = (recv_msg_t *)arg;
    output_dev_t *fdev = NULL;
    if (recv_msg->buffer == NULL)
    {
        pthread_exit(NULL);
    }

    if (recv_msg != NULL && recv_msg->buffer != NULL)
    {
        fdev = find_output_dev(ohead, recv_msg->buffer[2]);
    }
    ret = set_gpio_value(fdev);

    if (fdev != NULL)
    {
        fdev->gpio_value = recv_msg->buffer[3] - recv_msg->buffer[2] == 1 ? 0 : 1;
        printf("Current gpio value = %d\n", fdev->gpio_value);

        //! 灯
        if (recv_msg->buffer[2] == 0x10)
        {
            ret = set_gpio_value(fdev);
            if (fdev->gpio_value == LOW)
            {
                oled_putstrto(&disp, 0, 0, "lvroom's light is on");
            }
            else if (fdev->gpio_value == HIGH)
            {
                oled_putstrto(&disp, 0, 0, "lvroom's light is off");
            }
            oled_send_buffer(&disp);
            sleep(2);
            oled_clear(&disp);
        }

        if (recv_msg->buffer[2] == 0x20)
        {
            ret = set_gpio_value(fdev);
            if (fdev->gpio_value == LOW)
            {
                oled_putstrto(&disp, 0, 0, "bedroom's light is on");
            }
            else if (fdev->gpio_value == HIGH)
            {
                oled_putstrto(&disp, 0, 0, "bedroom's light is off");
            }
            oled_send_buffer(&disp);
            sleep(2);
            oled_clear(&disp);
        }

        if (recv_msg->buffer[2] == 0x30)
        {
            ret = set_gpio_value(fdev);
            if (fdev->gpio_value == LOW)
            {
                oled_putstrto(&disp, 0, 0, "bath's light is on");
            }
            else if (fdev->gpio_value == HIGH)
            {
                oled_putstrto(&disp, 0, 0, "bath's light is off");
            }
            oled_send_buffer(&disp);
            sleep(2);
            oled_clear(&disp);
        }

        if (recv_msg->buffer[2] == 0x40)
        {
            ret = set_gpio_value(fdev);
            if (fdev->gpio_value == LOW)
            {
                oled_putstrto(&disp, 0, 0, "pool's light is on");
            }
            else if (fdev->gpio_value == HIGH)
            {
                oled_putstrto(&disp, 0, 0, "pool's light is off");
            }
            oled_send_buffer(&disp);
            sleep(2);
            oled_clear(&disp);
        }

        //! 烟雾
        if (recv_msg->buffer[2] == 0x00)
        {   
            ret = set_gpio_value(fdev);
            if (recv_msg->buffer[3] == 0x01)
            {
                pthread_t tid_smoke;
                p = recv_msg->globalDev->phead;
                p = find_input_dev(p, "voice_dev");
                oled_putstrto(&disp, 0, 45 + 5, "warning: fire detected !");
                oled_send_buffer(&disp);
                pthread_create(&tid_smoke, NULL, p->sendRoutine, (void *)recv_msg->buffer);
                sleep(5);
                oled_clear(&disp);
            }
            else if (recv_msg->buffer[3] == 0x00)
            {
                fdev->gpio_value = HIGH;
                ret = set_gpio_value(fdev);
            }
        }
    }

    //! 人脸识别
    if (recv_msg->buffer[2] == 0x50 && recv_msg->buffer[3] == 0x51)
    {   
        pthread_t tid_face;
        pthread_create(&tid_face, NULL, face_routine, (void *)recv_msg);
    }

    //! 灯全开全关
    if (recv_msg->buffer[2] == 0x60)
    {

        if (recv_msg->buffer[3] == 0x61)
        {
            printf("all open\n");
            set_all_low(ohead);
            oled_putstrto(&disp, 0, 0, "all lights are on");
            oled_send_buffer(&disp);
            sleep(2);
            oled_clear(&disp);
        }
        else if (recv_msg->buffer[3] == 0x60)
        {
            printf("all close\n");
            set_all_hight(ohead);
            oled_putstrto(&disp, 0, 0, "all lights are off");
            oled_send_buffer(&disp);
            sleep(2);
            oled_clear(&disp);
        }
    }
    //! 温湿度
    if (recv_msg->buffer[0] == 'R' && recv_msg->buffer[1] == 'H')
    {
        oled_putstrto(&disp, 0, 18 + 2, "DHT11 DATE:");
        oled_putstrto(&disp, 0, 27 + 3, recv_msg->buffer);
        oled_send_buffer(&disp);
    }

    pthread_exit(NULL);
}

static void *recv_interface_recv(void *arg)
{
    struct mq_attr attr;
    recv_msg_t *recv_msg = NULL;
    if (NULL != arg)
    {
        recv_msg = malloc(sizeof(recv_msg_t));
        recv_msg->globalDev = (globalValue_t *)arg;
        recv_msg->msg_len = -1;
        recv_msg->buffer = NULL;
    }
    else
    {
        pthread_exit(NULL);
    }

    if (mq_getattr(recv_msg->globalDev->mqd, &attr) == -1) //! 获取消息队列属性, 通过全局变量mqd获取
    {
        pthread_exit(NULL);
    }
    recv_msg->buffer = malloc(attr.mq_msgsize);
    char *buff = recv_msg->buffer;
    memset(recv_msg->buffer, 0, attr.mq_msgsize);
    struct timespec timeout = {.tv_sec = 5, .tv_nsec = 0};
    // #if 0
    pthread_t tid;
    while (1)
    {
        int bytesLen = mq_timedreceive(recv_msg->globalDev->mqd,
                                       recv_msg->buffer, attr.mq_msgsize,
                                       NULL,
                                       &timeout); //! 设置超时时间，防止一直等待阻塞
        // printf("%s|%s|%d:send 0x%x, 0x%x,0x%x, 0x%x, 0x%x,0x%x\n", __FILE__, __func__, __LINE__, buff[0], buff[1], buff[2], buff[3], buff[4], buff[5]);
        // printf("%s|%s|%d:read_len=%d\n", __FILE__, __func__, __LINE__, bytesLen);
        if (bytesLen == -1) // 接收消息
        {
            if (errno == EAGAIN) // 消息队列为空
            {
                // do nothing
                continue;
            }
            else if (errno == ETIMEDOUT) // 超时
            {
                printf("timeout\n");
                continue;
            }
            else
            {
                break;
            }
        }
        else if (bytesLen > 0)
        {

            recv_msg->msg_len = strlen(buff);
            if (recv_msg->msg_len > 0)
            {

                recv_msg->msg_len = bytesLen;
                if (buff[0] == 0xAA && buff[1] == 0x55)
                    printf("%x,%x,%x,%x,%x,%x\n", buff[0], buff[1], recv_msg->buffer[2], buff[3], buff[4], buff[5]);

                pthread_create(&tid, NULL, msg_handle, (void *)recv_msg);
            }
        }
    }

    pthread_exit(NULL);
    // #endif
}

static void *recv_interface_send(void *arg)
{
    // do nothing
    return NULL;
}

input_dev_t recv_routine = {
    .input_dev_name = "recv_routine",
    .init = recv_interface_init,
    .final = recv_interface_final,
    .recvRoutine = recv_interface_recv,
    .sendRoutine = recv_interface_send,
    .next = NULL};

input_dev_t *add_recv_list(input_dev_t *head)
{
    return add_input_dev(head, &recv_routine);
}
