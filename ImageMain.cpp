#include "ImageCapture.h"

int main()
{
    ImageCapture *g_pclImageCapture = new ImageCapture;

    if(g_pclImageCapture->Init()) {
        return -1;
    }
    if (g_pclImageCapture->MainStart()) {
        return -2;
    }

    //while(1) {
        sleep(2);
    //}

    return 0;
}

