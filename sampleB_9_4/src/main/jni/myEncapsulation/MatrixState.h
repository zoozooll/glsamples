#ifndef MatrixState_h
#define MatrixState_h
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "Matrix.h"
class MatrixState
{
private:

	static float mProjMatrix[16];//4x4矩阵 投影用
    static float mVMatrix[16];//摄像机位置朝向9参数矩阵
    static float mMVPMatrix[16];//获取具体物体的总变换矩阵
    static float currMatrix[16];//当前变换矩阵
    static float mVMatrixForSpecFrame[16];//摄像机位置朝向9参数矩阵
    //保护变换矩阵的栈
    static float mStack[10][16];
    static int stackTop;
public:

    static GLfloat* cameraFB;
    static GLfloat* lightPositionFBSun;
    
    static void setInitStack();//获取不变换初始矩阵
    
    static void pushMatrix();//保护变换矩阵
    
    static void popMatrix();//恢复变换矩阵
    
    static void copyMVMatrix();//用于一帧内的摄像机矩阵

    static void translate(float x,float y,float z);//设置沿xyz轴移动
    
    static void rotate(float angle,float x,float y,float z);//设置绕xyz轴移动
    
    static void scale(float x,float y,float z);//设置沿xyz轴缩放
    //设置摄像机
    static void setCamera
    (
     	float cx,	//摄像机位置x
		float cy,   //摄像机位置y
		float cz,   //摄像机位置z
		float tx,   //摄像机目标点x
		float ty,   //摄像机目标点y
		float tz,   //摄像机目标点z
		float upx,  //摄像机UP向量X分量
		float upy,  //摄像机UP向量Y分量
		float upz   //摄像机UP向量Z分量
     );
    //设置透视投影参数
    static void setProjectFrustum
    (
		float left,		//near面的left
		float right,    //near面的right
		float bottom,   //near面的bottom
		float top,      //near面的top
		float near,		//near面距离
		float far       //far面距离
     );

    //设置灯光位置的方法
    static void setLightLocation(float x,float y,float z);

    static float* getFinalMatrix();//获取具体物体的总变换矩阵
    static float* getMMatrix();//获取具体物体的变换矩阵
    static float* getProjMatrix();
    static float* getCaMatrix();
};


#endif
