#define NOMINMAX

#include "openniDevice.h"
#include <fstream>
#include <string>

// Globals
// ユーザトラッキングの色
unsigned char colors[5][3] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {127, 127, 0},
    {127, 0, 127}
};

// コンストラクタ
OpenNIDevice::OpenNIDevice() {
}

// デストラクタ
OpenNIDevice::~OpenNIDevice() {
}

/**
 * デバイスの初期化.
 */
int OpenNIDevice::OpenDevice() {
        XnStatus rc;

        EnumerationErrors errors;
        rc = context.InitFromXmlFile(SAMPLE_XML_PATH, &errors);
        if (rc == XN_STATUS_NO_NODE_PRESENT) {
                XnChar strError[1024];
                errors.ToString(strError, 1024);
                printf("%s\n", strError);
                return (-1);
        } else if (rc != XN_STATUS_OK) {
                printf("Open failed: %s\n", xnGetStatusString(rc));
                return (-1);
        }

        rc = context.FindExistingNode(XN_NODE_TYPE_DEPTH, depth);
        rc = context.FindExistingNode(XN_NODE_TYPE_IMAGE, image);
        rc = user.Create(context);

        depth.GetMetaData(depthMD);
        image.GetMetaData(imageMD);

        // Hybrid mode isn't supported in this sample
        if (imageMD.FullXRes() != depthMD.FullXRes() || imageMD.FullYRes() != depthMD.FullYRes()) {
                printf ("The device depth and image resolution must be equal!\n");
                return -1;
        }

        // RGB is the only image format supported.
        if (imageMD.PixelFormat() != XN_PIXEL_FORMAT_RGB24) {
                printf("The device image format must be RGB24\n");
                return -1;
        }

        depth.GetAlternativeViewPointCap().SetViewPoint(image);  // imageとdepthのズレを自動で合わせる
        context.SetGlobalMirror(FALSE);   // FALSEで通常画像 デフォルトでは左右反転画像
        width = (int)depthMD.FullXRes();
        height = (int)depthMD.FullYRes();

        XnFieldOfView FOV;
        depth.GetFieldOfView(FOV);
        fXToZ = tan(FOV.fHFOV / 2) * 2;
        fYToZ = tan(FOV.fVFOV / 2) * 2;

        context.StartGeneratingAll();

        return 0;
}

/**
 * 画面サイズを取得.
 */
int OpenNIDevice::InqSize(int *x, int *y) {
        *x = width;
        *y = height;
        return 0;
}

/**
 * デバイスの終了処理.
 */
int OpenNIDevice::CloseDevice(void)
{
        context.Shutdown();
        return 0;
};

/**
 * 画素のBGRデータ・深度データを出力．
 * imageBuffer, depthRawBufferに格納される.
 */
void OpenNIDevice::GetData(unsigned char *imageBuffer, unsigned short *depthRawBuffer, unsigned char *userBuffer) {
        XnStatus rc = XN_STATUS_OK;

        // Read a new frame
        rc = context.WaitAnyUpdateAll();
        if (rc != XN_STATUS_OK) {
                printf("Read failed: %s\n", xnGetStatusString(rc));
                return;
        }

        depth.GetMetaData(depthMD);
        image.GetMetaData(imageMD);
        user.GetUserPixels(0, sceneMD);

        // image. -------------------------------------------------------------
        const XnRGB24Pixel* pImageRow = imageMD.RGB24Data();
        unsigned char* pBuffRow = imageBuffer;
        for (XnUInt y = 0; y < imageMD.YRes(); ++y) {
                const XnRGB24Pixel* pImage = pImageRow;
                unsigned char* pBuff = pBuffRow;

                for (XnUInt x = 0; x < imageMD.XRes(); x++, pImage++, pBuff += 3) {
                        pBuff[0] = pImage->nBlue;
                        pBuff[1] = pImage->nGreen;
                        pBuff[2] = pImage->nRed;
                }

                pImageRow += imageMD.XRes();
                pBuffRow += imageMD.XRes() * 3;
        }
        // --------------------------------------------------------------------

        // depth. -------------------------------------------------------------
        xnOSMemSet(depthRawBuffer, 0, sizeof(depthRawBuffer));

        const XnDepthPixel* pDepthRow = depthMD.Data();
        unsigned short* pDepthBuffRow = depthRawBuffer;
        for (XnUInt y = 0; y < depthMD.YRes(); ++y) {
                const XnDepthPixel* pDepth = pDepthRow;
                unsigned short* pBuff = pDepthBuffRow;

                for (XnUInt x = 0; x < depthMD.XRes(); x++, pDepth++, pBuff++) {
                        if (*pDepth != 0) {
                                *pBuff = *pDepth;
                        }
                }

                pDepthRow += depthMD.XRes();
                pDepthBuffRow += depthMD.XRes();
        }
        // --------------------------------------------------------------------

        // user. --------------------------------------------------------------
        const XnLabel *label = sceneMD.Data();
        unsigned char *pUserBuffRow = userBuffer;
        for (XnUInt y = 0; y < depthMD.YRes(); y++) {
                unsigned char* pBuff = pUserBuffRow;

                for (XnUInt x = 0; x < depthMD.XRes(); x++, label++, pBuff++) {
                        *pBuff = (unsigned char)(*label);
                }

                pUserBuffRow += depthMD.XRes();
        }
        // --------------------------------------------------------------------
}

/** 
 * DrawArrayを生成する.
 * 描画する分だけデータをoutputArrayDataに格納する
 * 現在の格納されたデータ数を返す
 */
unsigned int OpenNIDevice::createDrawArrays(unsigned char *imageBuffer, unsigned short *depthRawBuffer, unsigned char *userBuffer, unsigned char *vertexArray, int mode) {
        
        unsigned char *pImageBuffer = imageBuffer;
        unsigned short *pDepthBuffer = depthRawBuffer;
        unsigned char  *pUserBuffer = userBuffer;
        memset((void *)vertexArray, 0, width * height * ((sizeof(char) * 4 + sizeof(float) * 3)));
        unsigned char *pArray = vertexArray;
        int count = 0;
        for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
            // 深度0のとき描画しない
                        if ((*pDepthBuffer) != 0) {
                                int userlabel = (int)(*pUserBuffer);
                                if (mode == 1 && userlabel != 0 && userlabel < 6) {
                                        // ユーザ検出モード
                                        pArray[0] = colors[userlabel - 1][0];
                                        pArray[1] = colors[userlabel - 1][1];
                                        pArray[2] = colors[userlabel - 1][2];
                                        pArray[3] = 0;
                                } else {
                                        pArray[0] = pImageBuffer[2];  // R
                                        pArray[1] = pImageBuffer[1];  // G
                                        pArray[2] = pImageBuffer[0];  // B
                                        pArray[3] = 0;
                                }

                                // 座標情報を格納
                                float fNormalizedX = ((float)x / width - 0.5f);
                                float fNormalizedY = (0.5f - (float)y / height);
                                float tmpX = (float)(fNormalizedX * (float)(*pDepthBuffer) * fXToZ);
                                float tmpY = (float)(fNormalizedY * (float)(*pDepthBuffer) * fYToZ);
                                float tmpZ = (float)(*pDepthBuffer);

                                *((float *) (pArray + 4)) = tmpX;
                                *((float *) (pArray + 4 + sizeof(float))) = tmpY;
                                *((float *) (pArray + 4 + sizeof(float) * 2)) = tmpZ;
                                pArray += sizeof(char) * 4 + sizeof(float) * 3;
                                count++;
                        }
                        pImageBuffer += 3;
                pDepthBuffer++;
                pUserBuffer++;
                }
        }
        return count;
}

/** 
 *  Depthカメラ(raw)->ワールド座標（単位mm）
 * 入力：
 *　 x:参照型 float :DepthカメラX座標 0～639
 *　 y:参照型 float :DepthカメラY座標 0～479
 *　 z:参照型 float :DepthカメラDepth 0～
 * 出力：
 *　 x:参照型 float :ワールド座標X（単位mm）
 *　 y:参照型 float :ワールド座標Y（単位mm）
 *　 z:参照型 float :ワールド座標Z（単位mm）
 */
void OpenNIDevice::KinectDepthToWorld(float &x, float &y, float &z) {
        XnPoint3D in[1],out[1];

        in[0].X = x;
        in[0].Y = y;
        in[0].Z = z;
        depth.ConvertProjectiveToRealWorld (1,in,out);
        x = out[0].X;
        y =-out[0].Y; //ARToolKitのカメラ座標系は、Y軸が下向き
        z = out[0].Z;
};

void OpenNIDevice::KinectDepthToWorldArray(int num, XnPoint3D* inArray, XnPoint3D* outArray) {
        depth.ConvertProjectiveToRealWorld ((XnUInt32)num, inArray, outArray);
}