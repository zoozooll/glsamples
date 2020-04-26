/*
 * MS3DModel.h
 *
 *  Created on: 2015-9-7
 *      Author: Administrator
 */

#ifndef MS3DMODEL_H_
#define MS3DMODEL_H_

#include <vector>
#include <map>
#include <jni.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "MS3DHeader.h"
#include "MS3DVertex.h"
#include "MS3DTriangle.h"
#include "MS3DGroup.h"
#include "MS3DMaterial.h"
#include "MS3DJoint.h"

using namespace std;

class MS3DModel{
private:
	void updateVectexs();//动画中更新顶点数据的方法
	void updateVectex(int index);//更新特定顶点的方法
	void initBuffer();//初始化缓冲

public:
	int num_ver;//顶点数量
	int num_gro;//组数量
	int num_joi;//关节数量

	int textId;

	float fps;		//fps信息
	float current_time;	//当前时间
	float totalTime;	//总时间
	int frame_count;	//关键帧数

	vector<GLfloat*> vertexCoordingBuffer;//顶点坐标数据缓冲
	vector<GLfloat*> texCoordingBuffer;	//纹理坐标数据缓冲
	MS3DHeader* header;				//头信息
	vector<MS3DVertex*> vertexs;			//顶点信息
	vector<MS3DTriangle*> triangles;		//三角形索引
	vector<MS3DGroup*> groups;			//组信息
	vector<MS3DMaterial*> materials;		//材质信息(目前用到的主要是纹理部分)
	vector<MS3DJoint*> joints;				//关节信息
	static map<string,int> textureManager;//纹理管理器

	GLuint mProgram;//自定义渲染管线程序id
	GLuint muMVPMatrixHandle;//总变换矩阵引用id
	GLuint maPositionHandle; //顶点位置属性引用id
	GLuint maTexCoorHandle; //顶点纹理坐标属性引用id

	MS3DModel(JNIEnv* env,jobject obj);
	~MS3DModel();

	void initShader();//初始化着色器
	void draw(bool isUpdate);//绘制模型
	void animate(float ttime);//进行动画的方法
	void updateJoint(float ttime);//更新关节的方法
//	void ppt();
	GLfloat* updateVertexs(int triangleCount,int* indexs);

};



#endif /* MS3DMODEL_H_ */
