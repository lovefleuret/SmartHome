#ifndef SMOKE_H
#define SMOKE_H
#include <wiringPi.h>
#include <stdio.h>

#include "input_dev.h"
#include "unistd.h"
#include "global.h"
#include "msg_queue.h"
#include "oled.h"

input_dev_t *add_smoke_dev(input_dev_t *head);

#endif /* SMOKE_H */
