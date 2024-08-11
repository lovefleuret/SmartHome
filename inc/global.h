#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "input_dev.h"
#include "msg_queue.h"
typedef struct global
{
    mqd_t mqd;
    input_dev_t *phead;
} globalValue_t;

typedef struct recv_msg
{
    int msg_len;
    unsigned char *buffer;
    globalValue_t *globalDev;
} recv_msg_t;

extern struct display_info disp;

#endif /*  */
