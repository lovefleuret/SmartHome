#ifndef INPUT_DEV_H
#define INPUT_DEV_H
#include <string.h>

typedef struct input_dev
{
    char input_dev_name[32];
    int (*init)(void);
    void (*final)(void);
    void *(*recvRoutine)(void *arg);
    void *(*sendRoutine)(void *arg);

    struct input_dev *next;
} input_dev_t;

input_dev_t *add_input_dev(input_dev_t *phead, input_dev_t *pdev);
input_dev_t *find_input_dev(input_dev_t *phead, char *name);

#endif /* __INPUT_DEV_LIST_H */
