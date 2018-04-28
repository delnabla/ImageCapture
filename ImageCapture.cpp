/******************************************************************************
 * Copyright (C)
 * File name:       
 * Version:         
 * Description:    
 * Other:
 * History:
 
 ******************************************************************************/

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

#include "ImageCapture.h"


ImageCapture::ImageCapture()
{

}

ImageCapture::~ImageCapture()
{
    //munmap(p_map, 10485760);
}

S32 ImageCapture::Open()
{
    fd = open((const char*)CameraDev, O_RDWR);
    if (fd == -1) {
        printf("open camera: %s error\n", CameraDev);
        return -1;
    }

    return 0;
}

S32 ImageCapture::Open(const char *dev)
{
    fd = open(dev, O_RDWR);
    if (fd == -1) {
        printf("open camera: %s error\n", dev);
        return -1;
    }

    return 0;
}

S32 ImageCapture::Close()
{
    if (fd) {
        close(fd);
    }

    return 0;
}

S32 ImageCapture::GetInformation()
{
    ioctl(fd, VIDIOC_QUERYCAP, &cap);  
    printf("\nDriverName:%s/nCard Name:%s/nBus info:%s/nDriverVersion:%u.%u.%u\n", cap.driver,cap.card,cap.bus_info,(cap.version>>16)&0XFF,(cap.version>>8)&0XFF,cap.version&0XFF);

    fmtDesc.index = 0;  
    fmtDesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("Support Format:\n");
    while(ioctl(fd, VIDIOC_ENUM_FMT,&fmtDesc) != -1)
    {
        printf("\t%d.%c%c%c%c\t%s\n",
               fmtDesc.index + 1, fmtDesc.pixelformat & 0xFF, (fmtDesc.pixelformat >> 8) & 0xFF, (fmtDesc.pixelformat >> 16) & 0xFF, (fmtDesc.pixelformat >> 24) & 0xFF, fmtDesc.description);
        fmtDesc.index++;
    }

    //查询当前摄像头的视频格式 VIDIOC_G_FMT
    fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fd, VIDIOC_G_FMT, &fmt);
    printf("Current Camera Format:\n");
    printf("\tfmt.type is %d\n", fmt.type);
    printf("\tWidth = %d\n", fmt.fmt.pix.width);
    printf("\tHeight= %d\n", fmt.fmt.pix.height);
    printf("\tSizeimage = %d\n", fmt.fmt.pix.sizeimage);
    printf("\tPixFormat = %c%c%c%c\n", fmt.fmt.pix.pixelformat & 0xFF, fmt.fmt.pix.pixelformat >> 8 & 0xFF,fmt.fmt.pix.pixelformat >> 16 & 0xFF,fmt.fmt.pix.pixelformat >> 24 & 0xFF);

    return 0;
}

S32 ImageCapture::SetParameters()
{
    printf("Set Camera Parameters \n");
    fps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fps.parm.capture.timeperframe.numerator = 1;
    fps.parm.capture.timeperframe.denominator = 30;
    ioctl(fd, VIDIOC_S_PARM, &fps);
    ioctl(fd, VIDIOC_G_PARM, &fps);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = s32ImageWidth;
    fmt.fmt.pix.height = s32ImageHeight;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    // fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YVU420;
    //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;
    //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_SRGGB10;
    //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    ioctl(fd, VIDIOC_S_FMT, &fmt);
    if ((fmt.fmt.pix.width != s32ImageWidth) || (fmt.fmt.pix.height != s32ImageHeight))
        printf("Warning: driver is sending image at %d x %d\n", fmt.fmt.pix.width, fmt.fmt.pix.height);
    else {
        printf("fmt.type is %d\n", fmt.type);
        printf("Width = %d\n", fmt.fmt.pix.width);
        printf("Height= %d\n", fmt.fmt.pix.height);
        printf("Sizeimage = %d\n", fmt.fmt.pix.sizeimage);
    }

    return 0;
}

S32 ImageCapture::InitV4L2MMap()
{
    req.count = 4;  
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    req.memory = V4L2_MEMORY_MMAP;  
    ioctl(fd,VIDIOC_REQBUFS, &req); 

    buffers =(buffer*)calloc(req.count, sizeof(*buffers));
    if (!buffers) {  
        fprintf (stderr,"Out of memory\n");  
        return -1;
    } 

    //MMAP，VIDIOC_QUERYBUF
    for (unsigned int n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;
        // Query buffer and get the start address
        if (-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf)) {
            return -1;
        }
        buffers[n_buffers].length = buf.length;
        //Do mmap
        buffers[n_buffers].start = mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        if (MAP_FAILED == buffers[n_buffers].start) {
            return -1;
        }
    }

    return 0;
}
S32 ImageCapture::StartCapture()
{
    //Put empty buffer to video input buffer VIDIOC_QBUF
    for (int i = 0; i < req.count; ++i) {
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        ioctl(fd, VIDIOC_QBUF, &buf);
    }

    printf("Start Video Capture\n");
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fd, VIDIOC_STREAMON, &type);

    return 0;
}

S32 ImageCapture::StopCapture()
{
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == ioctl(fd, VIDIOC_STREAMOFF, &type)) {
        return -1;
    }

    return 0;
}

S32 ImageCapture::ReadOneFrame()
{
    CLEAR(buf);

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    
    if (-1 == ioctl(fd, VIDIOC_DQBUF, &buf)) {
        return -1;
    }

    ProcessImage(buffers[buf.index].start, buf.bytesused);

    if (-1 == ioctl(fd, VIDIOC_QBUF, &buf)) {
        return -2;
    }

    return 0;
}

S32 ImageCapture::ProcessImage(const void *p, int size)
{
    
    if (j > 0) {
        return 0;
    }
    printf("size %d\n", size);
    fwrite(p, size, 1, fp[j++]);

    //memcpy(p_map->data, p, size);
    //p_map->size = size;

    return 0;
}

S32 ImageCapture::MainStart()
{
    int s32Ret;

    i = 0;
    j = 0;

    //printf("\nCamera0: \n\tDevice Path: %s \n\tWith: %d \n\tHeight: %d\n\n",
    //       g_pclImageConfig->stSysConfig.strCamera0DevPath.c_str(),
    //       g_pclImageConfig->stSysConfig.u32Camera0With,
    //       g_pclImageConfig->stSysConfig.u32Camera0Height);
            
    //Open(g_pclImageConfig->stSysConfig.strCamera0DevPath.c_str());
    if (Open("/dev/video0")) {
        //printf("open /dev/video0 device failed\n");
        return -1;
    }

    s32ImageWidth = 3264;//640;//g_pclImageConfig->stSysConfig.u32Camera0With;//3264;//640;
    s32ImageHeight = 2448;//480;//g_pclImageConfig->stSysConfig.u32Camera0Height;//2448;//480;

    GetInformation();

    SetParameters();

    GetInformation();

    InitV4L2MMap();

    StartCapture();

    s32Ret = pthread_create((pthread_t*)&threadHandler, NULL, WorkerRun, this);
    if (0 != s32Ret) {
        printf("Create Image Capture Thread error\n");
        return -2;
    }
}

void *ImageCapture::WorkerRun(void *arg)
{
    ImageCapture *pclImageCapture;

    /// set thread parameter.
    pthread_detach(pthread_self());

    pclImageCapture = reinterpret_cast<ImageCapture *>(arg);

    //BaseTraceOut(g_u32MCUTraceID, BASE_TRACE_INFO, 
    printf(    "Thread <WorkerRun> is Running ...\n");

    pclImageCapture->MainLoop();

    //BaseTraceOut(g_u32MCUTraceID, BASE_TRACE_INFO,
    printf(     "Thread <WorkerRun> is End !!!\n");

    return NULL;
}

S32 ImageCapture::MainLoop()
{
    printf("main loop\n");
    ///while (1) {
        ReadOneFrame();
        //BaseTimeSleep(1000);
        //sleep(1);
        printf("Image Capture Loop\n");
    //}
    return 0;
}

S32 ImageCapture::Init()
{
    char s[] = "file.raw";
    char filename[32] = { 0 };

    for (i = 0; i < 1; i++) {
        snprintf(filename, sizeof(filename), "/tmp/%d%s", i, s);
        printf("i = %d %s \n ", i, filename);
        fp[i] = fopen(filename, "w");
    }


    fd = open("/tmp/cam_mmap",O_CREAT | O_RDWR | O_TRUNC, 00777);
    lseek(fd, sizeof(struct ImageData) - 2, SEEK_SET);
    write(fd, "", 1);
    p_map = (struct ImageData*)mmap(NULL, sizeof(struct ImageData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    return 0;
}

