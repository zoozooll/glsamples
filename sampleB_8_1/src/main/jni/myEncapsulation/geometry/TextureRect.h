#ifndef TextureRect__h
#define TextureRect__h

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

class TextureRect {
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

    int vCount;
    float UNIT_SIZE;
public:
    float mFlagK;//纹理属性
    TextureRect(
    		float halfWidth, //半宽
    		float halfHeight, //半高
    		float offset, //位移量
    		float UNIT_SIZE//单位长度
			);//xoy面上的一个矩形
    void initVertexDataRectangle(
    		float halfWidth, //半宽
    		float halfHeight, //半高
    		float offset, //位移量   一般为0
    		float UNIT_SIZE//单位长度
    		);
    void initShader();
    void drawSelf(const GLint texId);
};

#endif
