#ifndef SOCKET_H
#define SOCKET_H
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> /* See NOTES */
#include <arpa/inet.h>
#include <errno.h>

#include "input_dev.h"
#include "global.h"


input_dev_t *add_socket_dev(input_dev_t *phead);

#endif /* SOCKET_H */
