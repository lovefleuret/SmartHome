#include "socket.h"

#define PORT 8888
int sockFd = -1;
struct sockaddr_in serverAddr;
static int socket_init()
{
    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    ret = listen(sockFd, 5);
    if (ret == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return sockFd;
}

static void sock_close()
{
    close(sockFd);
}

static void *sock_get(void *arg)
{   
    globalValue_t *sockDev = (globalValue_t *)arg;
    socklen_t len = sizeof(serverAddr);
    char buffer[6] = {0xAA , 0x55, 0x00, 0x00, 0x55, 0xAA};
    //! connect
    while (1)
    {
        
        int client_Fd = accept(sockFd, (struct sockaddr *)&serverAddr, &len);

        if (client_Fd == -1)
        {
            perror("accept");
            break;
        }
        else
        {
            printf("client connected......\n");
        }
        //! send  这边只能连一个客户端，连多个需要创建线程
        while (1)
        {
            int bytes = recv(client_Fd, buffer, 6, 0);

            if (bytes == -1)
            {
                perror("recv");
                break;
            }
            else if (bytes == 0)
            {
                printf("client disconnected\n");
                break;
            }
            else if (bytes > 0)
            {
                msg_queue_send(sockDev->mqd, buffer, 6);
            }
        }
    }

    pthread_exit(NULL);
}

static void *socket_routine(void *arg)
{
    // do nothing
}

input_dev_t sock_dev = {
    .input_dev_name = "socket_dev",
    .init = socket_init,
    .final = sock_close,
    .recvRoutine = sock_get,
    .sendRoutine = NULL,
    .next = NULL};

input_dev_t *add_socket_dev(input_dev_t *phead)
{
    return add_input_dev(phead, &sock_dev);
}