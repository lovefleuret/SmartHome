/*
 *  ┌─────────────────────────────────────────────────────────────┐
 *  │┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐│
 *  ││Esc│!1 │@2 │#3 │$4 │%5 │^6 │&7 │*8 │(9 │)0 │_- │+= │|\ │`~ ││
 *  │├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┤│
 *  ││ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │{[ │}] │ BS  ││
 *  │├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤│
 *  ││ Ctrl │ A │ S │ D │ F │ G │ H │ J │ K │ L │: ;│" '│ Enter  ││
 *  │├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┤│
 *  ││ Shift  │ Z │ X │ C │ V │ B │ N │ M │< ,│> .│? /│Shift │Fn ││
 *  │└─────┬──┴┬──┴──┬┴───┴───┴───┴───┴───┴──┬┴───┴┬──┴┬─────┴───┘│
 *  │      │Fn │ Alt │         Space         │ Alt │Win│   HHKB   │
 *  │      └───┴─────┴───────────────────────┴─────┴───┘          │
 *  └─────────────────────────────────────────────────────────────┘
 */
#include "mqueue.h"
#include "global.h"
#include "voice_dev.h"
#include "input_dev.h"
#include "output_dev.h"
#include "smoke.h"
#include "socket.h"
#include "recv_interface.h"
#include "DHT11.h"

#include <wiringPi.h>
enum inputDev
{
    voice,
    sock,
    smoke,
    recv_routine,
    DHT11,
    devNum
};

int init_something()
{   
    if (wiringPiSetup() == -1)
    {
        printf("wiringPi setup failed!\n");
        return -1;
    }
    return 0;
}   
int main(int argc, char *argv[])
{
    globalValue_t *usrKey = NULL;
    usrKey = malloc(sizeof(globalValue_t));
    usrKey->mqd = -1;
    usrKey->phead = NULL;
    int numOfDev = 0;

    init_something();
    usrKey->mqd = msg_queue_create();
    
    usrKey->phead = add_voice_dev(usrKey->phead);
    usrKey->phead = add_socket_dev(usrKey->phead);
    usrKey->phead = add_smoke_dev(usrKey->phead);
    usrKey->phead = add_recv_list(usrKey->phead);
    usrKey->phead = add_DHT1_dev(usrKey->phead);

    input_dev_t *pointer = usrKey->phead;

    while (pointer != NULL)
    {
        if (pointer->init != NULL)
        {
            printf("dev name: %s\n", pointer->input_dev_name);
            pointer->init();
        }
        pointer = pointer->next;
        numOfDev++;
    }
    pthread_t tid[numOfDev];

#if DEBUG
    printf("--------------%s-----------------%d---All devices were initialized successfully ~_~\n", __func__, __LINE__);
#endif
    pointer = usrKey->phead;
    for (int i = 0; i < numOfDev; i++)
    {
        if (pointer != NULL)
        {
            pthread_create(&tid[i], NULL, (void *)pointer->recvRoutine, (void *)usrKey);
        }
        pointer = pointer->next;
    }

    for (int i = 0; i < numOfDev; i++)
    {
        pthread_join(tid[i], NULL);
    }

    free(usrKey);

    //! 关闭所有设备
    pointer = usrKey->phead;
    for (int i = 0; i < numOfDev; i++)
    {
        if (pointer != NULL)
        {
            pointer->final();
        }
        pointer = pointer->next;
    }
    return 0;
}