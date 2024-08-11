#include "output_dev.h"
#if 0
output_dev_t *add_output_dev(output_dev_t *phead, output_dev_t *pnew)
{

    if (phead == NULL)
    {
        phead = pnew;
    }
    else
    {
        //! 用头插法
        pnew->next = phead;
        phead = pnew;
    }
    return phead;
}

#endif

output_dev_t *find_output_dev(output_dev_t *phead, int key)
{
    //! 查找结点
    output_dev_t *p = phead;
    while (p != NULL)
    {
        if (p->key == key)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int set_gpio_value(output_dev_t *pdev)
{
    if (pdev == NULL)
    {
        return -1;
    }

    if (pdev->gpio_pin != -1)
    {
        pinMode(pdev->gpio_pin, pdev->gpio_mode);
    }

    if (pdev->gpio_value != -1)
    {
        digitalWrite(pdev->gpio_pin, pdev->gpio_value);
    }
    return 0;
}

int set_all_low(output_dev_t *phead)
{
    output_dev_t *p = phead;

    while (p != NULL)
    {
        pinMode(p->gpio_pin, p->gpio_mode);
        if (p->key != 0x00)
            digitalWrite(p->gpio_pin, LOW);
        p = p->next;
    }
    return 0;
}

int set_all_hight(output_dev_t *phead)
{
    output_dev_t *p = phead;
    while (p != NULL)
    {
        pinMode(p->gpio_pin, p->gpio_mode);
        if (p->key != 0X00)
            digitalWrite(p->gpio_pin, HIGH);
        p = p->next;
    }
    return 0;
}

int set_sg90_turn(int pwm_pin)
{   
    
    pinMode(pwm_pin, OUTPUT);

    softPwmCreate(pwm_pin, 0, 200); // range 为200，周期20ms  0到200。

    softPwmWrite(pwm_pin, 23); // 1.5ms 就是180度  10/200= 0.05

    delay(1000);

    // 停止输入pwm信号
    softPwmStop(pwm_pin);
    return 0;
}

int reset_sg90(int pwm_pin)
{
    pinMode(pwm_pin, OUTPUT);

    softPwmCreate(pwm_pin, 0, 200); // range 为200，周期20ms  0到200。

    softPwmWrite(pwm_pin, 5); // 0.5ms 就是0度  10/200= 0.05

    delay(1000);

    // 停止输入pwm信号
    softPwmStop(pwm_pin);
    return 0;
}