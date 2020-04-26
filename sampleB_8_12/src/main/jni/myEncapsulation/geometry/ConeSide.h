#ifndef _ConeSide_H_
#define _ConeSide_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../Bullet/LinearMath/btVector3.h"

//圆柱的侧面
class ConeSide
{
    GLuint mProgram;//自定义渲染管线程序id
    GLuint muMVPMatrixHandle;//总变换矩阵引用id
    GLuint muMMatrixHandle;//位置、旋转变换矩阵
    GLuint mTexHandle;//外观纹理属性引用id

    GLuint mFragK;//外观纹理属性引用id

    GLuint maCameraHandle; //摄像机位置属性引用id
    GLuint maLightLocationHandle;//光源位置属性引用
    GLuint maPositionHandle; //顶点位置属性引用id
    GLuint maTexCoorHandle; //顶点纹理坐标属性引用id
    GLuint maNormalHandle;//顶点法向量属性引用id

    const GLvoid* mVertexBuffer;	//顶点坐标数据缓冲
    const GLvoid* mTextureBuffer;	//顶点纹理坐标数据缓冲
    const GLvoid* mNormalBuffer;	//顶点法向量数据缓冲

    int 		texId;			//纹理id
    float 		TRIANGLE_SIZE;	//三角形的单位大小
    int vCount;//顶点总数 有重复
public:
    float mFlagK;//纹理属性
	ConeSide(
			int texIdIn,
			float r,//半径
			int n,//切分的份数
			float h//圆柱的高度
			);//轴为y轴的圆柱侧边
//	~ConeSide();

    void initShader();
    void drawSelf(int texIdIn);
    btVector3* method(
    		btVector3* vertices,//顶点数组(有重复值的)
    		int nums_vertices,//顶点数组的长度
    		int* index_vertices,//顶点数组的索引数组
    		int nums_indexVertices,//顶点数组的索引数组的长度	注意：应该与nums_vertices相同
    		int nums_noRepeat//顶点数目，无重复顶点的数目
    		);
};

#endif
