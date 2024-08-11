#ifndef __OUTPUT_DEV_H
#define __OUTPUT_DEV_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "softPwm.h"

typedef struct output_dev
{
    char output_dev_name[32];
    int key;        //! 用来查找
    int gpio_pin;   //! gpio引脚号
    int gpio_mode;  //! gpio模式output 或则 input
    int gpio_value; //! gpio状态  LOw:低电平 HIGH:高电平
    int check_face_status;
    int voice_set_status;
    struct output_dev *next;
} output_dev_t;
//! 添加设备节点

// output_dev_t *add_output_dev(output_dev_t *phead, output_dev_t *pnew);
output_dev_t *find_output_dev(output_dev_t *phead, int key);
int set_gpio_value(output_dev_t *pdev);
int set_all_low(output_dev_t *phead);
int set_all_hight(output_dev_t *phead);
int reset_sg90(int pwm_pin);
int set_sg90_turn(int pwm_pin);


#endif /* __output_dev_H */
