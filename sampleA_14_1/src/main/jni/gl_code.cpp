#include <jni.h>//����ͷ�ļ�
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "util/MatrixState.h"
#include "util/FileUtil.h"
#include "Triangle.h"
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

Triangle *t;//�����ζ���ָ��

bool setupGraphics(int w, int h)//��ʼ������
{
    glViewport(0, 0, w, h);//�����ӿ�
    float ratio = (float) w/h;//�������
    MatrixState::setProjectFrustum(-ratio, ratio, -1, 1, 1, 10);//����ͶӰ����
    MatrixState::setCamera(0, 0, 3, 0, 0, 0, 0, 1, 0);//�������������
    MatrixState::setInitStack();//��ʼ���任����
    glClearColor(0.5f, 0.5f, 0.5f, 1);//���ñ�����ɫ
    t = new Triangle();	//���������ζ���
    return true;
}

void renderFrame() {	//��Ⱦ����
    glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����
    t->drawSelf();//����������
    MatrixState::rotate(1,1,0,0);//��x����תһ��
}
//��Ӧ��Java�Ǳߵı��ط�����ʵ��
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_bn_sample14_11_GL2JNILib_init
  (JNIEnv *, jclass, jint width, jint height)//���ó�ʼ������
{
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_bn_sample14_11_GL2JNILib_step
  (JNIEnv *, jclass)//������Ⱦ����
{
    renderFrame();
}

JNIEXPORT void JNICALL Java_com_bn_sample14_11_GL2JNILib_nativeSetAssetManager
(JNIEnv* env, jclass cls, jobject assetManager)//���ü�����ɫ���ű�����
{
	AAssetManager* aamIn = AAssetManager_fromJava( env, assetManager );//��ʼ��AAssetManager����
    FileUtil::setAAssetManager(aamIn);//����AAssetManager
}

#ifdef __cplusplus
}
#endif


