#include "openniDevice.h"
#include "glut.h"

#define WINDOW_X 640
#define WINDOW_Y 480

// ���[�h
enum modeList {
        NORMAL_MODE,
        USER_MODE
} MODE_LIST;

int windowX = WINDOW_X;
int windowY = WINDOW_Y;
int mode    = NORMAL_MODE;

int xsize, ysize; 
OpenNIDevice device;

// �e�f�[�^�̃o�b�t�@
static unsigned char gImageBuffer[640 * 480 * 3];
static unsigned short gDepthRawBuffer[640 * 480];
static unsigned char gUserBuffer[640 * 480];
// �`��p
static unsigned char gVertexArray[640 * 480 * (sizeof(char) * 4 + sizeof(float) * 3)];

// �֐��錾
void glutIdle(void);
void glutDisplay(void);
void glutKeyboard(unsigned char key, int x, int y);
void glutReshape(int w, int h);
void Cleanup(void);

/**
 * GL�A�C�h���֐�.
 */
void glutIdle (void) {
        // Display the frame
        glutPostRedisplay();
}

/**
 * �E�C���h�E�T�C�Y�ύX���̏���.
 */
void glutReshape(int w, int h) {
        glViewport(0, 0, w, h);
        windowX = w;
        windowY = h;
}

/**
 * �f�B�X�v���C�֐�.
 */
void glutDisplay(void) {
        // �f�[�^�̎擾
        unsigned char *pImageBuffer = gImageBuffer;
        unsigned short *pDepthBuffer = gDepthRawBuffer;
        unsigned char *pUserBuffer = gUserBuffer;
        unsigned char *pVertexArray = gVertexArray;
        device.GetData(pImageBuffer, pDepthBuffer, pUserBuffer);

        // vertex array ����
        int arraySize = device.createDrawArrays(pImageBuffer, pDepthBuffer, pUserBuffer, pVertexArray, mode);

        // �`�揀��
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Auto Rotate ���[���h���W�̉����������ɂ��ĉ�]
        static float xrot = 0.0f;
        glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(xrot, 0.0f, 1.0f, 0.0f);
        xrot += 1.0f;

        // �`�揈�� 
        glPushMatrix(); {
                // �|�C���g�N���E�h��`��
                glInterleavedArrays(GL_C4UB_V3F, 0, pVertexArray);
                glDrawArrays(GL_POINTS, 0, arraySize);

        } glPopMatrix();

        // �`��o�b�t�@�X�V
        glutSwapBuffers();
}

/**
 * �L�[�{�[�h.
 */
void glutKeyboard(unsigned char key, int x, int y) {
#ifdef _DEBUG  // ���̓L�[��\��
        printf("[%c] key was pressed\n", key);
#endif

        switch (key) {
        case 0x1b:      // ESC�L�[����͂�����A�v���P�[�V�����I��
                Cleanup();
                exit(0);
                break;
        case 'u':
                mode = (mode++) % 2;
                if (mode == USER_MODE)
                        printf("user mode ON\n");
                else
                        printf("user mode OFF\n");
                break;
        default:
                break;
        }
}

/**
 * �I�������֐�.
 */
void Cleanup(void) {
        device.CloseDevice();
}

/**
 * ���C���֐�.
 */
int ni_pcl_main(int argc, char **argv) {
        // OpenNI init
        if (device.OpenDevice() < 0) exit(0);
        if (device.InqSize(&xsize, &ysize) < 0) exit(0);

        // OpenGL init
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowSize(windowX, windowY);
        glutCreateWindow("3D Point Cloud");

        glutKeyboardFunc(glutKeyboard);
        glutDisplayFunc(glutDisplay);
        glutIdleFunc(glutIdle);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (double)xsize / (double)ysize, 0.01, 100000.0);

        /// ���_�ʒu
        gluLookAt(0.0f, 0.0f, 3000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        glutMainLoop();
}