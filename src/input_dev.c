#include "input_dev.h"
#include <stdio.h>

input_dev_t *add_input_dev(input_dev_t *phead, input_dev_t *pnew)
{
    if(phead == NULL)
    {
        phead = pnew;
        return phead;
    }
    else
    {
        pnew->next = phead;
        phead = pnew;

        return phead;
    }
    return NULL;
}   

input_dev_t *find_input_dev(input_dev_t *phead, char *name)
{
    input_dev_t *p = phead;
    while(p != NULL)
    {
        if(strcmp(p->input_dev_name, name) == 0)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}
