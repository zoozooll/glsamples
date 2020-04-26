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

//地形  光线投射
extern "C" {
	float TOUCH_SCALE_FACTOR = 180.0f/320;//角度缩放比例
	float angle=45;//摄像机仰角
	float direction=0;//摄像机方位角
	float tdistance=25;//摄像机距离目标点的距离

	//摄像机的位置坐标
	float cx = 0;
	float cy = 7;
	float cz = -15;
	//目标点
	float tx=0;
	float ty=2;
	float tz=0;

	//up向量
	float upx = 0;
	float upy = 2;
	float upz = 0;

	float mTime = 0;	//当前时间（用于动画播放）


	//设置摄像机位置
	void setCamera(float dx,float dy)
	{
		direction += dx*TOUCH_SCALE_FACTOR;	//方位角
		angle+=dy*TOUCH_SCALE_FACTOR;			//方向角
		if(angle<=0)
		{
			angle=0;
		}else if(angle>=90)
		{
			angle=90;
		}

		//虚拟向量
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

		 //计算转轴向量
		 float zhou[3];
		 zhou[0]=-cup[2];
		 zhou[1]=0;
		 zhou[2]=cup[0];

		 //计算摄像机参数
		 float helpM[16];
		 Matrix::setIdentityM(helpM, 0);
		 Matrix::rotateM(helpM, 0, angle, zhou[0],zhou[1],zhou[2]);
		 float cupr[4];
		 float cLocationr[4];
		 Matrix::multiplyMV(cupr, 0, helpM, 0, cup, 0);
		 Matrix::multiplyMV(cLocationr, 0, helpM, 0, cLocation, 0);

		 cx=cLocationr[0];//摄像机位置
		 cy=cLocationr[1];
		 cz=cLocationr[2];
		 tx=0.0f;ty=0.0f;tz=0.0f;//观察点位置
		 upx=cupr[0];upy=cupr[1];upz=cupr[2];//up向量

	}

	MS3DModel* ms3d;		//ms3d模型指针对象

	bool onSurfaceChanged(int w, int h){
	    glViewport(0, 0, w, h);

	    float ratio = (float) w/h;
	    MatrixState::setProjectFrustum(-ratio, ratio, -1, 1, 1,1000);
	    //调用此方法产生摄像机矩阵
	    MatrixState::setCamera(cx, cy, cz, tx, ty, tz, upx, upy, upz);

	    //初始化基本变换矩阵
	    MatrixState::setInitStack();
		return true;
	}


	bool onSurfaceCreated(JNIEnv * env,jobject obj){

	    glClearColor(0, 0, 0,1); //设置屏幕背景色RGBA
	    glEnable(GL_DEPTH_TEST);//打开深度检测
	    glEnable(GL_CULL_FACE);//打开背面剪裁

		FileUtil::binaryData = FileUtil::loadBinary("ninja.ms3d");//加载.ms3d文件
		ms3d =new MS3DModel(env,obj);//创建ms3d模型指针对象
		return true;
	}

	void renderFrame() {
		//清除深度缓冲与颜色缓冲
	    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	    //设置摄像机
		MatrixState::setCamera(
			cx,cy,cz,
			tx,ty,tz,
			upx,upy,upz
		);

		float span=6.0f;//忍者模型间的间距
		int k=2;//忍者模型数量控制系数
		for(int i=-k;i<=k;i++){//对列循环绘制忍者模型
			for(int j=-k;j<=k;j++){//对行循环绘制忍者模型
				MatrixState::pushMatrix();//保护变换矩阵
		        MatrixState::translate(i*span, 0, j*span);//平移到指定的位置
		        ms3d->animate(mTime);//进行忍者模型的绘制
		        MatrixState::popMatrix();//恢复变换矩阵
			}
		}
		//更新模型动画时间
		mTime += 0.015f;
		//若当前播放时间大于总的动画时间则实际播放时间等于当前播放时间减去总的动画时间
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

