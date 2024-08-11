#include "msg_queue.h"

#define QUENAME "/test"

mqd_t msg_queue_create()
{
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 1024;
    attr.mq_curmsgs = 0;

    //! 打开消息队列文件
    mqd_t mqd = mq_open(QUENAME, O_CREAT | O_RDWR | O_NONBLOCK, 0666, &attr);
    if (mqd == (mqd_t)-1)  //! 也可以不用强制转换
    {
        perror("mq_open");
        exit(1);
    }

    return mqd;
}

int msg_queue_send(mqd_t mq, char *msg, int len)
{
    //! 发送消息
    int byte_send  = mq_send(mq, msg, len, 0);

    return byte_send;
}


void msg_queue_close(mqd_t mq)
{
    //! 关闭消息队列
    mq_close(mq);
    mq_unlink(QUENAME); //! 释放资源
    return ;
}




#if 0 //! test消息队列代码
struct mq_attr
{
    __syscall_slong_t mq_flags;   /* Message queue flags.  */
    __syscall_slong_t mq_maxmsg;  /* Maximum number of messages.  */
    __syscall_slong_t mq_msgsize; /* Maximum message size.  */
    __syscall_slong_t mq_curmsgs; /* Number of messages currently queued.  */
    __syscall_slong_t __pad[4];
};

#define MSG "hello world"
mqd_t mq;

void *msg_sender(void *arg)
{
#if 0
        fgets(buf, sizeof(buf), stdin);
        //! 发送消息
#endif

    for(int i = 0; i < 5; i++)
    {
        mq_send(mq, MSG, strlen(MSG), 0);
    }
    

}

void msg_receiver(union sigval v)
{
    char buf[1024];
    //! 接收消息
    while (1)
    {
        mq_receive(mq, buf, sizeof(buf), NULL);

        printf("recv: %s\n", buf);
    }

    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;          //! 通知方式为线程
    sev.sigev_notify_function = msg_receiver; //! 通知的函数 (触发函数)回调函数
    sev.sigev_notify_attributes = NULL;       //! 通知的属性
    sev.sigev_value.sival_ptr = &mq;          //! 传递的参数

    mq_notify(mq, &sev); //! 通知  这里用到了一个递归函数
}


int main(int argc, char const *argv[])
{
    pthread_t sender, receiver;

    

    //! 打开成功，就开始收发消息
    pthread_create(&sender, NULL, msg_sender, NULL);

    if (0)
    {
        /* Structure to transport application-defined values with signals.  */
        typedef struct sigevent
        {
            __sigval_t sigev_value;
            int sigev_signo;
            int sigev_notify;

            union
            {
                int _pad[__SIGEV_PAD_SIZE];

                /* When SIGEV_SIGNAL and SIGEV_THREAD_ID set, LWP ID of the
                   thread to receive the signal.  */
                __pid_t _tid;

                struct
                {
                    void (*_function)(__sigval_t); /* Function to start.  */
                    pthread_attr_t *_attribute;    /* Thread attributes.  */
                } _sigev_thread;
            } _sigev_un;
        } sigevent_t;
    }

    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;          //! 通知方式为线程
    sev.sigev_notify_function = msg_receiver; //! 通知的函数 (触发函数)回调函数
    sev.sigev_notify_attributes = NULL;       //! 通知的属性
    sev.sigev_value.sival_ptr = &mq;          //! 传递的参数

    mq_notify(mq, &sev); //! 通知

    pthread_join(sender, NULL);

    //! 等待别人把消息读走
    sleep(1);
    //! 关闭消息队列
    mq_close(mq);
    mq_unlink("/test"); //! 释放资源
    return 0;
}
#endif