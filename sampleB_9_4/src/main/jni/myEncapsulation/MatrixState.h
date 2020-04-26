#ifndef MatrixState_h
#define MatrixState_h
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "Matrix.h"
class MatrixState
{
private:

	static float mProjMatrix[16];//4x4���� ͶӰ��
    static float mVMatrix[16];//�����λ�ó���9��������
    static float mMVPMatrix[16];//��ȡ����������ܱ任����
    static float currMatrix[16];//��ǰ�任����
    static float mVMatrixForSpecFrame[16];//�����λ�ó���9��������
    //�����任�����ջ
    static float mStack[10][16];
    static int stackTop;
public:

    static GLfloat* cameraFB;
    static GLfloat* lightPositionFBSun;
    
    static void setInitStack();//��ȡ���任��ʼ����
    
    static void pushMatrix();//�����任����
    
    static void popMatrix();//�ָ��任����
    
    static void copyMVMatrix();//����һ֡�ڵ����������

    static void translate(float x,float y,float z);//������xyz���ƶ�
    
    static void rotate(float angle,float x,float y,float z);//������xyz���ƶ�
    
    static void scale(float x,float y,float z);//������xyz������
    //���������
    static void setCamera
    (
     	float cx,	//�����λ��x
		float cy,   //�����λ��y
		float cz,   //�����λ��z
		float tx,   //�����Ŀ���x
		float ty,   //�����Ŀ���y
		float tz,   //�����Ŀ���z
		float upx,  //�����UP����X����
		float upy,  //�����UP����Y����
		float upz   //�����UP����Z����
     );
    //����͸��ͶӰ����
    static void setProjectFrustum
    (
		float left,		//near���left
		float right,    //near���right
		float bottom,   //near���bottom
		float top,      //near���top
		float near,		//near�����
		float far       //far�����
     );

    //���õƹ�λ�õķ���
    static void setLightLocation(float x,float y,float z);

    static float* getFinalMatrix();//��ȡ����������ܱ任����
    static float* getMMatrix();//��ȡ��������ı任����
    static float* getProjMatrix();
    static float* getCaMatrix();
};


#endif
