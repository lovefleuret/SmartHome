#ifndef VOICE_DEV_H
#define VOICE_DEV_H


static int voice_dev_init(void);
static void voice_dev_final(void);
static void *voice_get(void *arg);
static void *voice_set(void *arg);

input_dev_t *add_voice_dev(input_dev_t *phead);

#endif /* VOICE_DEV_H */
