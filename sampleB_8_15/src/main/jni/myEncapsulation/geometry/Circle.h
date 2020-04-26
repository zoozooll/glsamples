#ifndef _Circle_H_
#define _Circle_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../../Bullet/LinearMath/btVector3.h"

class Circle
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



    int m_vCount;//顶点总数 有重复
    bool needRefresh;
    btVector3* m_pos;//位置 指针
    int m_index;//位置索引值  			m_pos[m_index]表示圆中心点位置
    int m_texId;

	float* m_vertices;//坐标数据
	float* m_normal;//法向量数据
	float* m_textures;//顶点纹理S、T坐标值数组
	int m_n;//切分份数
	float m_r;//半径
public:
    float mFlagK;//纹理属性
	Circle(
			int texIdIn,
			float m_r,//半径
			int m_n,//圆被切割的份数    30
			btVector3* m_pos,
			int m_index
			);//xoz面上的圆   是朝上的圆
	Circle(
			int texIdIn,
			float m_r,//半径
			int m_n,//圆被切割的份数    30
			float h//圆的高度    0
			);//xoz面上的圆   是朝上的圆
    void initShader();
    void drawSelf(int m_texId);
    void refresh();
};

#endif
