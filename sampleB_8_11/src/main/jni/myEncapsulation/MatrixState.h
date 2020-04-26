#ifndef MatrixState_h
#define MatrixState_h
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "Matrix.h"
class MatrixState
{
private:

	static float mProjMatrix[16];
    static float mVMatrix[16];
    static float mMVPMatrix[16];
public:
    static float currMatrix[16];
    static float mStack[10][16];
    static int stackTop;
    static GLfloat* cameraFB;
    static GLfloat* lightPositionFBSun;
    
    static void setInitStack();
    
    static void pushMatrix();
    
    static void popMatrix();
    
    static void translate(float x,float y,float z);
    
    static void rotate(float angle,float x,float y,float z);
    
    static void scale(float x,float y,float z);
    static void setCamera
    (
     float cx,
     float cy,
     float cz,
     float tx,
     float ty,
     float tz,
     float upx,
     float upy,
     float upz
     );

    static void setProjectFrustum
    (
     float left,
     float right,
     float bottom,
     float top,
     float near,
     float far
     );
    
    //设置灯光位置的方法
    static void setLightLocation(float x,float y,float z);

    static float* getFinalMatrix();
    static float* getMMatrix();
    static float* getProjMatrix();
    static float* getCaMatrix();
};


#endif
