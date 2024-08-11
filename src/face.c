#include "face.h"

double face_category(void)
{
    double result = 0.0;
    sleep(2);
    system(WGET_CMD);
    if (0 != access(SEARCHFACE_FILE, F_OK))
    {
        return result;
    }

    PyObject *pModule = PyImport_ImportModule("face"); // face.py
    if (!pModule)
    {
        PyErr_Print();
        printf("Error: failed to load face.py\n");
        goto FAILED_MODULE;
    }

    PyObject *pFunc = PyObject_GetAttrString(pModule, "alibaba_face"); // alibaba_face函数
    if (!pFunc)
    {
        PyErr_Print();
        printf("Error: failed to load alibaba_face\n");
        goto FAILED_FUNC;
    }

    PyObject *pValue = PyObject_CallObject(pFunc, NULL); // 调用alibaba_face函数
    if (!pValue)
    {
        PyErr_Print();
        printf("Error: function call failed\n");
        goto FAILED_VALUE;
    }

    if (!PyArg_Parse(pValue, "d", &result)) // 解析获取调用alibaba_face函数的返回值，转行成c语言格式
    {
        PyErr_Print();
        printf("Error: parse failed");
        goto FAILED_VALUE;
    }

    printf("result=%0.2lf\n", result);

FAILED_VALUE:
    Py_DECREF(pValue);
FAILED_FUNC:
    Py_DECREF(pFunc);
FAILED_MODULE:
    Py_DECREF(pModule);

    return result;
}

void face_init(void)
{
    Py_Initialize();
    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString("."));
}

void face_final(void)
{
    Py_Finalize();
    return ;
}

void *face_routine(void *arg)
{
    pthread_detach(pthread_self());
    recv_msg_t *face_msg = (recv_msg_t *)arg;
    double face_result = 0.0;
    pthread_t tid_face;
    face_result = face_category();
    char buffer[6] = {0xAA, 0x55, 0x00, 0x00, 0x55, 0xAA};
    printf("face_result=%0.2lf\n", face_result);
    input_dev_t *p = face_msg->globalDev->phead;
    p = find_input_dev(p, "voice_dev");
    if (face_result >= 0.6) // 人脸识别成功
    {   
        buffer[2] = 0x02;
        buffer[3] = 0x03;
        set_sg90_turn(PWMPIN);
        pthread_create(&tid_face, NULL, p->sendRoutine, (void *)buffer);
        oled_putstrto(&disp, 0, 45 + 5, "open home's door !");
        oled_send_buffer(&disp);
        
        sleep(5);
        reset_sg90(PWMPIN);
        oled_clear(&disp);
    }
    else if (face_result < 0.6) // 人脸识别失败
    {   
        buffer[2] = 0x03;
        buffer[3] = 0x04;
        oled_putstrto(&disp, 0, 45 + 5, "Face recognition failed !");
        oled_send_buffer(&disp);
        pthread_create(&tid_face, NULL, p->sendRoutine, (void *)buffer);
        
        sleep(2);
        oled_clear(&disp);
    }

    pthread_exit(NULL);
}
