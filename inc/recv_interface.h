#ifndef RECV_INTERFACE_H
#define RECV_INTERFACE_H
#include <pthread.h>

#include "output_dev.h"
#include "msg_queue.h"
#include "global.h"
#include "input_dev.h"
#include "oled.h"
#include "face.h"
#include "myOled.h"
#include "ini.h"

#define PWMPIN 10

input_dev_t *add_recv_list(input_dev_t *head);

#endif /* RECV_INTERFACE_H */
