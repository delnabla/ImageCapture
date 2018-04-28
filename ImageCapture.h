#ifndef _IMAGE_CAPTURE_H_
#define _IMAGE_CAPTURE_H_

#include "ECOAppSysTypeDefine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <pthread.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

typedef struct buffer {
    void *start;
    size_t length;
}IMAGE_BUFFER;

struct ImageData
{
    char data[1024*1024*10];
    int size;
};

class ImageCapture {
public:
    ImageCapture();
    ~ImageCapture();

/*****************************************************************************
 Function
 *****************************************************************************/

    S32 Open();
    S32 Open(const char *dev);
    S32 Close();
    S32 Init();
    S32 GetInformation();
    S32 SetParameters();
    S32 InitV4L2MMap();
    S32 StartCapture();
    S32 StopCapture();
    S32 ReadOneFrame();
    S32 ProcessImage(const void *p, int size);
    S32 MainStart();

private:
    static void *WorkerRun(void *arg);
    S32 MainLoop();

    struct v4l2_capability cap;
    struct v4l2_fmtdesc fmtDesc;
    struct v4l2_streamparm fps;
    struct v4l2_format fmt;
    struct v4l2_requestbuffers req;
    struct v4l2_buffer buf;
    enum v4l2_buf_type type;
    IMAGE_BUFFER *buffers;
    struct ImageData camImage;
    struct ImageData *p_map;

    pthread_t threadHandler;

    unsigned char *CameraDev;
    int fd;
    int i;
    int j;
    S32 s32ImageWidth;
    S32 s32ImageHeight;

    FILE *fp[20];
    

};

#endif
