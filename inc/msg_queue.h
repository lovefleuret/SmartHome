#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>

mqd_t msg_queue_create() ;
int msg_queue_send(mqd_t mq, char *msg, int len) ;
void msg_queue_close(mqd_t mq);

#endif /* MSG_QUEUE_H */
