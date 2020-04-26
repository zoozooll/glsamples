#include <stdio.h>
#include <math.h>
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <vector>
#include "myEncapsulation/MatrixState.h"
#include "../myEncapsulation/FileUtil.h"

#include "../draw/MS3DModel.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

using namespace std;

//����  ����Ͷ��
extern "C" {
	float TOUCH_SCALE_FACTOR = 180.0f/320;//�Ƕ����ű���
	float angle=45;//���������
	float direction=0;//�������λ��
	float tdistance=25;//���������Ŀ���ľ���

	//�������λ������
	float cx = 0;
	float cy = 7;
	float cz = -15;
	//Ŀ���
	float tx=0;
	float ty=2;
	float tz=0;

	//up����
	float upx = 0;
	float upy = 2;
	float upz = 0;

	float mTime = 0;	//��ǰʱ�䣨���ڶ������ţ�


	//���������λ��
	void setCamera(float dx,float dy)
	{
		direction += dx*TOUCH_SCALE_FACTOR;	//��λ��
		angle+=dy*TOUCH_SCALE_FACTOR;			//�����
		if(angle<=0)
		{
			angle=0;
		}else if(angle>=90)
		{
			angle=90;
		}

		//��������
		 double angleHD=direction*0.017453;

		 float cup[4];
		 cup[0]=-(float)sin(angleHD);
		 cup[1]=0;
		 cup[2]=(float)cos(angleHD);
		 cup[3]=1;

		 float cLocation[4];
		 cLocation[0]=0;
		 cLocation[1]=tdistance;
		 cLocation[2]=0;
		 cLocation[3]=1;

		 //����ת������
		 float zhou[3];
		 zhou[0]=-cup[2];
		 zhou[1]=0;
		 zhou[2]=cup[0];

		 //�������������
		 float helpM[16];
		 Matrix::setIdentityM(helpM, 0);
		 Matrix::rotateM(helpM, 0, angle, zhou[0],zhou[1],zhou[2]);
		 float cupr[4];
		 float cLocationr[4];
		 Matrix::multiplyMV(cupr, 0, helpM, 0, cup, 0);
		 Matrix::multiplyMV(cLocationr, 0, helpM, 0, cLocation, 0);

		 cx=cLocationr[0];//�����λ��
		 cy=cLocationr[1];
		 cz=cLocationr[2];
		 tx=0.0f;ty=0.0f;tz=0.0f;//�۲��λ��
		 upx=cupr[0];upy=cupr[1];upz=cupr[2];//up����

	}

	MS3DModel* ms3d;		//ms3dģ��ָ�����

	bool onSurfaceChanged(int w, int h){
	    glViewport(0, 0, w, h);

	    float ratio = (float) w/h;
	    MatrixState::setProjectFrustum(-ratio, ratio, -1, 1, 1,1000);
	    //���ô˷����������������
	    MatrixState::setCamera(cx, cy, cz, tx, ty, tz, upx, upy, upz);

	    //��ʼ�������任����
	    MatrixState::setInitStack();
		return true;
	}


	bool onSurfaceCreated(JNIEnv * env,jobject obj){

	    glClearColor(0, 0, 0,1); //������Ļ����ɫRGBA
	    glEnable(GL_DEPTH_TEST);//����ȼ��
	    glEnable(GL_CULL_FACE);//�򿪱������

		FileUtil::binaryData = FileUtil::loadBinary("ninja.ms3d");//����.ms3d�ļ�
		ms3d =new MS3DModel(env,obj);//����ms3dģ��ָ�����
		return true;
	}

	void renderFrame() {
		//�����Ȼ�������ɫ����
	    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	    //���������
		MatrixState::setCamera(
			cx,cy,cz,
			tx,ty,tz,
			upx,upy,upz
		);

		float span=6.0f;//����ģ�ͼ�ļ��
		int k=2;//����ģ����������ϵ��
		for(int i=-k;i<=k;i++){//����ѭ����������ģ��
			for(int j=-k;j<=k;j++){//����ѭ����������ģ��
				MatrixState::pushMatrix();//�����任����
		        MatrixState::translate(i*span, 0, j*span);//ƽ�Ƶ�ָ����λ��
		        ms3d->animate(mTime);//��������ģ�͵Ļ���
		        MatrixState::popMatrix();//�ָ��任����
			}
		}
		//����ģ�Ͷ���ʱ��
		mTime += 0.015f;
		//����ǰ����ʱ������ܵĶ���ʱ����ʵ�ʲ���ʱ����ڵ�ǰ����ʱ���ȥ�ܵĶ���ʱ��
		if(mTime > ms3d->totalTime){
			mTime = mTime - ms3d->totalTime;
		}

	}

	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_setCamera
	  (JNIEnv *env, jclass jc,jfloat dx,jfloat dy)
	{
		setCamera(dx,dy);
	}

	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_nativeSetAssetManager
	  (JNIEnv *env, jclass cls, jobject assetManager)
	{
		AAssetManager* aamIn = AAssetManager_fromJava(env, assetManager);
	    FileUtil::setAAssetManager(aamIn);
	}

}

