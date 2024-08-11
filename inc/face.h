#ifndef FACE_H
#define FACE_H
#include <Python.h>
#include <string.h>

#include "global.h"
#include "recv_interface.h"
#include "input_dev.h"
#include "output_dev.h"

#define WGET_CMD "wget http://192.168.1.176:8080/?action=snapshot \
-O ./temp/SearchFace.jpg" //! 根据自己开发板的ip地址设置
#define SEARCHFACE_FILE "./temp/SearchFace.jpg"

double face_category(void);
void face_init(void);
void face_final(void);
void *face_routine(void *arg);

#endif /* FACE_H */
