#ifndef _DrawLine_H_
#define _DrawLine_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "../Bullet/LinearMath/btVector3.h"

class DrawLine
{
	GLuint mProgram;//自定义渲染管线程序id
	GLuint muMVPMatrixHandle;//总变换矩阵引用

    GLuint maColorHandle;
    GLuint maPositionHandle; //顶点位置属性引用

    const GLvoid* mVertexBuffer;//顶点坐标数据缓冲
    const GLvoid* mColorBuffer;//顶点纹理坐标数据缓冲

    float *colors;
    float *vertices;

    int vCount;//顶点总数  应该为2

public :
    DrawLine();
    DrawLine(
    		float *vertices//顶点坐标数组  长度应该为6
    		);
    DrawLine(
    		btVector3 vec0,//第一个顶点
    		btVector3 vec1//第二个顶点
    		);

    void initShader();
    void drawSelf();
    void initVertex(
    		btVector3 vec0,//第一个顶点
    		btVector3 vec1//第二个顶点
    		);
    void drawSelf(
    		btVector3 vec0,//第一个顶点
    		btVector3 vec1//第二个顶点
    		);

};

#endif
