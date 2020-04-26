#ifndef _AnyShapeNoTexture_H_
#define _AnyShapeNoTexture_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "../Bullet/LinearMath/btVector3.h"

class AnyShapeNoTexture
{
	GLuint mProgram;//自定义渲染管线程序id
	GLuint muMVPMatrixHandle;//总变换矩阵引用id

    GLuint maColorHandle;
    GLuint maPositionHandle; //顶点位置属性引用id

    const GLvoid* mVertexBuffer;//顶点坐标数据缓冲
    const GLvoid* mColorBuffer;//顶点纹理坐标数据缓冲

    float *colors;
    float *m_vertices;
    float *m_verticesLine;

    int vCount;//顶点总数  应该为2
    int vCountLine;


public :
    AnyShapeNoTexture(
    		float *m_vertices,//顶点*3坐标数组    有重复顶点的
    		int m_numsVer
    		);

    void initColor(int c);
    void initVertex(int f);

    void initShader();
    void drawSelf();

};

#endif
