#ifndef _CylinderSide_H_
#define _CylinderSide_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

//圆柱的侧面
class CylinderSide
{
    GLuint mProgram;//自定义渲染管线程序id
    GLuint muMVPMatrixHandle;//总变换矩阵引用
    GLuint muMMatrixHandle;//位置、旋转变换矩阵
    GLuint mTexHandle;//外观纹理属性引用

    GLuint mFragK;//外观纹理属性引用
    GLuint maCameraHandle; //摄像机位置属性引用
    GLuint maLightLocationHandle;//光源位置属性引用
    GLuint maPositionHandle; //顶点位置属性引用
    GLuint maTexCoorHandle; //顶点纹理坐标属性引用
    GLuint maNormalHandle;//顶点法向量属性引用

    const GLvoid* mVertexBuffer;	//顶点坐标数据缓冲
    const GLvoid* mTextureBuffer;	//顶点纹理坐标数据缓冲
    const GLvoid* mNormalBuffer;	//顶点法向量数据缓冲

    int 		texId;			//纹理id
    float 		TRIANGLE_SIZE;	//三角形的单位大小
    int vCount;//顶点总数 有重复
public:
    float mFlagK;//纹理属性
	CylinderSide(
			int texIdIn,
			float r,//半径
			int n,//切分的份数
			float h//圆柱的高度
			);//轴为y轴的圆柱侧边
//	~CylinderSide();

    void initShader();
    void drawSelf(int texIdIn);
};

#endif
