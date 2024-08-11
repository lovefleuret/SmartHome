#ifndef DHT11_H
#define DHT11_H

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input_dev.h"
#include "oled.h"
#include "global.h"

input_dev_t *add_DHT1_dev(input_dev_t *phead);

#endif /* DHT11_H */
