#ifndef _PR_DEVICE
#define _PR_DEVICE

#include "XnOS.h"
#include "XnCppWrapper.h"

using namespace xn;

// Defines
#define SAMPLE_XML_PATH "../data/SamplesConfig.xml"

    class OpenNIDevice {
    private:
        Context context;
        DepthGenerator depth;
        ImageGenerator image;
                UserGenerator user;
        DepthMetaData depthMD;
        ImageMetaData imageMD;
                SceneMetaData sceneMD;

        int width, height;

    public:
        double fXToZ, fYToZ;

        OpenNIDevice();
        ~OpenNIDevice();
        int OpenDevice();  // デバイス初期化
        int InqSize(int *x, int *y);
        void GetData(unsigned char *imageBuffer, unsigned short *depthRawBuffer, unsigned char *userBuffer);
                unsigned int createDrawArrays(unsigned char *imageBuffer, unsigned short *depthRawBuffer, unsigned char *userBuffer, unsigned char *vertexArray, int mode);

        void KinectDepthToWorld(float &x, float &y, float &z);
        void KinectDepthToWorldArray(int num, XnPoint3D* inArray, XnPoint3D* outArray);

        int CloseDevice(void);

    };

#endif // _PR_DEVICE
