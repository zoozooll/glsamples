/*
 * MS3DModel.cpp
 *
 *  Created on: 2015-9-7
 *      Author: Administrator
 */
#include "MS3DModel.h"
#include "../myEncapsulation/FileUtil.h"
#include "../myEncapsulation/ShaderUtil.h"
#include "myEncapsulation/MatrixState.h"
#include "../element/Mat4.h"
#include <string>
#include <map>

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

using namespace std;
map<string,int> MS3DModel::textureManager;
MS3DModel::MS3DModel(JNIEnv * env,jobject obj)
{
	initShader();//初始化着色器

	//加载模型
	header = new MS3DHeader();	//加载头信息

	num_ver=FileUtil::myReadUnsignedShort();//读取顶点数量
	for(int i=0; i<num_ver; i++){//循环读取每个顶点的信息
		MS3DVertex* vertex = new MS3DVertex();
		vertexs.push_back(vertex);
	}
	int num_tri=FileUtil::myReadUnsignedShort();//读取三角形数量
	for(int i=0; i<num_tri; i++)//循环加载每一个三角形的组装索引信息
	{
		MS3DTriangle* triangle= new MS3DTriangle();
		triangles.push_back(triangle);
	}
	//加载组信息
	num_gro=FileUtil::myReadUnsignedShort();//读取组数量
	for(int i=0; i<num_gro; i++)//循环加载每个组的信息
	{
		MS3DGroup* group = new MS3DGroup();
		groups.push_back(group);
	}
	//加载材质信息
	int num_mat=FileUtil::myReadUnsignedShort();//读取材质数量
	for(int i=0; i<num_mat; i++)//循环加载每个材质的信息
	{
		MS3DMaterial* mal = new MS3DMaterial(env,obj);
		materials.push_back(mal);
	}
	fps=FileUtil::myReadFloat();//加载帧速率信息
	current_time = FileUtil::myReadFloat();//当前时间
	frame_count = FileUtil::myReadInt();//关键帧数
	totalTime = frame_count / fps;//计算动画总时间
	//加载关节信息
	num_joi=FileUtil::myReadUnsignedShort();//获取关节数量
	map<string, MS3DJoint*> mapp;
	for(int i = 0; i < num_joi; i++){//循环加载每个关节的信息
		MS3DJoint* joint = new MS3DJoint();

		mapp[joint->name]=joint;//将关节信息对象存储进map以备查找
		//判断是否有父关节
		map<string,MS3DJoint*>::iterator iter=mapp.begin();
		for(; iter != mapp.end(); iter++)
		{
			string pname=iter->first;
			if(pname==(joint->parentName))
			{
				joint->parent = mapp[joint->parentName]; //获得此关节的父关节
				joint->ifparent=true;
				break;
			}
		}
		joint->relative =new Mat4();//设置相对矩阵
		joint->relative->loadIdentity();
		//设置旋转
		joint->relative->genRotationFromEulerAngle(joint->trotate);
		joint->relative->setTranslation(joint->tposition); //设置平移
		joint->absolute = new Mat4();//设置绝对矩阵
		joint->absolute->loadIdentity();
		if(joint->ifparent){//是否有父关节
			//有父关节的话绝对矩阵等于父关节的绝对矩阵乘以子关节的相对矩阵
			joint->absolute->mul(joint->parent->absolute,joint->relative);
		}else{
			//无父关节的话相对矩阵即为绝对矩阵
			joint->absolute->copyFrom(joint->relative);
		}

		joints.push_back(joint);
	}
	mapp.clear();//清除map

	initBuffer();//初始化缓冲
}

MS3DModel::~MS3DModel()
{
	//删除MS3DHeader对象
	delete header;

	//删除所有MS3DVertex对象
	vector<MS3DVertex*>::iterator iter=vertexs.begin();
	for(;iter!=vertexs.end();iter++)
	{
		MS3DVertex* vt=(*iter);
		delete vt;
	}
	vertexs.clear();

	//删除所有MS3DTriangle对象
	vector<MS3DTriangle*>::iterator iter0=triangles.begin();
	for(;iter0!=triangles.end();iter0++)
	{
		MS3DTriangle* vt=(*iter0);
		delete vt;
	}
	triangles.clear();

	//删除所有MS3DGroup对象
	vector<MS3DGroup*>::iterator iter1=groups.begin();
	for(;iter1!=groups.end();iter1++)
	{
		MS3DGroup* vt=(*iter1);
		delete vt;
	}
	groups.clear();

	//删除所有MS3DMaterial对象
	vector<MS3DMaterial*>::iterator iter2=materials.begin();
	for(;iter2!=materials.end();iter2++)
	{
		MS3DMaterial* vt=(*iter2);
		delete vt;
	}
	materials.clear();
	//删除所有MS3DJoint对象
	vector<MS3DJoint*>::iterator iter3=joints.begin();
	for(;iter3!=joints.end();iter3++)
	{
		MS3DJoint* vt=(*iter3);
		delete vt;
	}
	joints.clear();
}

//初始化着色器
void MS3DModel::initShader()
{
	//加载顶点着色器的脚本
	string strVertex = FileUtil::loadShaderStr("vertex.sh");
	const char* anyShapeVertex = strVertex.c_str();
	//加载片元着色器的脚本
	string strFragment = FileUtil::loadShaderStr("frag.sh");
	const char* anyShapeFragment = strFragment.c_str();

	//基于顶点着色器与片元着色器创建程序
	mProgram = ShaderUtil::createProgram(anyShapeVertex, anyShapeFragment);
	//获取程序中总变换矩阵引用id
	muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
	//顶点位置属性id
	maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
	//获取程序中顶点颜色属性引用id
	maTexCoorHandle = glGetAttribLocation(mProgram, "aTexCoor");

}
//绘制方法
void MS3DModel::draw(bool isUpdate)
{
	glUseProgram(mProgram); //指定使用某套shader程序
	MatrixState::copyMVMatrix();
	//将最终变换矩阵传入shader程序
	glUniformMatrix4fv(muMVPMatrixHandle,1,0,MatrixState::getFinalMatrix());//将最终变换矩阵传入shader程序
	//启用顶点坐标数组
	MS3DGroup* group;
	MS3DMaterial* material;   //材质
	int* indexs;

	vector<MS3DGroup*>::iterator iter1=groups.begin();
	vector<GLfloat*>::iterator iter_vcb=vertexCoordingBuffer.begin();
	vector<GLfloat*>::iterator iter_tcb=texCoordingBuffer.begin();
	vector<MS3DMaterial*>::iterator iter2;

	for(;iter1!=groups.end();iter1++,iter_tcb++,iter_vcb++)
	{
		group =(*iter1);	//获取当前组信息对象
		indexs = group->getIndicies();//获取组内三角形的索引数组
		int triangleCount  =group->getCount_ind();//获取组内三角形的数量
		//有材质（这里主要是指需要贴纹理）
		if(group->getMaterialIndex() > -1)
		{
			iter2=materials.begin();
			for(int t=0;t<(group->getMaterialIndex());t++)
			{
				iter2++;
			}
			material = (*iter2);
			//绑定纹理
			textId=MS3DModel::textureManager[material->getName()];
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textId);

			//参数：程序中顶点纹理属性引用id,一组有2个数，数的类型，GL_FALSE，一组所占字节数，顶点纹理的首地址
			glVertexAttribPointer(maTexCoorHandle,2,GL_FLOAT,GL_FALSE,2*4,(*iter_tcb));
			//启用纹理坐标数组
			glEnableVertexAttribArray(maTexCoorHandle);
		}
		if(isUpdate)
		{//更新顶点缓冲
			(*iter_vcb)=updateVertexs(triangleCount, indexs);
		}
		//参数：程序中顶点位置属性引用id,一组有3个数，数的类型，GL_FALSE，一组所占字节数，顶点数据的首地址
		glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3*4, (*iter_vcb));
		glEnableVertexAttribArray(maPositionHandle);
		//用顶点法进行绘制
		glDrawArrays(GL_TRIANGLES, 0, triangleCount*3);

		glDisableVertexAttribArray(maPositionHandle);
		glDisableVertexAttribArray(maTexCoorHandle);

	}
}

//进行动画的方法
void MS3DModel::animate(float ttime)
{
	if(current_time != ttime){//相同时间不做更新
		updateJoint(ttime);	//更新关节
		updateVectexs();	//更新顶点
		draw(true);	//执行绘制
	}else{
		//执行绘制
		draw(false);
	}
}
//更新关节的方法
void MS3DModel::updateJoint(float ttime)
{
	current_time = ttime;//更新当前时间
	if(current_time > totalTime){//时间超过总时间置为零
		current_time = 0.0f;
	}
	vector<MS3DJoint*>::iterator iter=joints.begin();
	for(;iter!=joints.end();iter++){//更新每个关节
		MS3DJoint* vt=(*iter);
		vt->update(current_time);
	}

}
//动画中更新顶点数据的方法
void MS3DModel::updateVectexs()
{
	int count =num_ver;//获取顶点数量
	for(int i=0; i<count; i++){	//更新每个顶点
		updateVectex(i);
	}
}
//更新特定顶点的方法
void MS3DModel::updateVectex(int index)
{
	//获取当前需要更新的顶点对应的顶点信息对象
	vector<MS3DVertex*>::iterator iter=vertexs.begin();
	for(int i=0;i<index;i++)
	{
		iter++;
	}
	MS3DVertex* vt = (*iter);
	//是否有关节骨骼ID
	if(vt->getBone() == -1){//无关节控制
		vt->setCurrPosition(vt->getInitPosition());
	}else{//有关节控制
		vector<MS3DJoint*>::iterator iter3=joints.begin();
		int it=vt->getBone();
		for(int i=0;i<it;i++)
		{
			iter3++;
		}
		MS3DJoint* joint =(*iter3);//获取对应的关节
		//根据关节的实时变换情况计算出顶点经关节影响后的位置
		vt->setCurrPosition(joint->getMatrix()->transform(joint->getAbsolute()->invTransformAndRotate(vt->getInitPosition())));
	}
}
GLfloat* MS3DModel::updateVertexs(int triangleCount,int* indexs)
{
	MS3DTriangle* triangle;
	int* vertexIndexs;	//顶点索引
	//更新顶点缓冲
	GLfloat* buffers = new float[triangleCount*9];	//buffer缓冲
	int num_v=0;

	vector<MS3DTriangle*>::iterator iter0;
	vector<MS3DVertex*>::iterator iter;
	for(int j=0; j<triangleCount; j++)
	{//对组内的每个三角形循环
		iter0=triangles.begin();
		for(int t=0;t<indexs[j];t++)
		{
			iter0++;
		}
		triangle = (*iter0);//获取当前要处理的三角形信息对象
		vertexIndexs = triangle->getIndexs();	//获取三角形中三个顶点的顶点索引
		//将三角形的三个顶点的数据放入顶点数据缓冲（实际是完成三角形的组装）

		for(int k=0; k<3; k++)
		{
			iter=vertexs.begin();
			for(int t=0;t<vertexIndexs[k];t++)
			{
				iter++;
			}

			MS3DVertex* mvt=(*iter);
			buffers[num_v++]=mvt->getCurrPosition()->getVector3fArray()[0];
			buffers[num_v++]=mvt->getCurrPosition()->getVector3fArray()[1];
			buffers[num_v++]=mvt->getCurrPosition()->getVector3fArray()[2];
		}
	}
	return buffers;
}
//初始化缓冲
void MS3DModel::initBuffer()
{
	//将关节更新到起始时间（时间为0的时间）
	updateJoint(0.0f);
	updateVectexs();//更新顶点坐标
	int triangleCount = 0;//每组三角形个数
	MS3DGroup* group;//临时组信息
	MS3DTriangle* triangle;//临时三角形信息
	int* indexs;//三角形索引
	int* vertexIndexs;//顶点索引

	//对模型中的每个组进行循环处理
	vector<MS3DGroup*>::iterator iter1=groups.begin();
	for(;iter1!=groups.end();iter1++)
	{

		group = (*iter1);//获取当前要处理的组
		indexs = group->getIndicies();  //获取组内三角形索引数组
		triangleCount =group->getCount_ind();//获取组内三角形的数量

		GLfloat* texCoordingBuffer0=new float[triangleCount*6];//数据缓冲
		GLfloat* vertexCoordingBuffer0=new float[triangleCount*9];

		vector<MS3DTriangle*>::iterator iter0;

		int num_tc=0;
		int num_vc=0;

		//循环对组内的每个三角形进行处理
		for(int j=0; j<triangleCount; j++)
		{
			iter0=triangles.begin();
			for(int k=0;k<indexs[j];k++)
			{
				iter0++;

			}
			triangle = (*iter0);//获取当前要处理的三角形
			vertexIndexs = triangle->getIndexs();//获取三角形中各个顶点的索引
			vector<MS3DVertex*>::iterator iter;
			//对三角形中的三个顶点进行循环处理
			for(int k=0; k<3; k++)
			{
				//将当前遍历到的顶点的纹理ST坐标送入缓冲
				texCoordingBuffer0[num_tc++]=triangle->getS()->getVector3fArray()[k];
				texCoordingBuffer0[num_tc++]=triangle->getT()->getVector3fArray()[k];
				//获取当前组的顶点坐标数据缓冲
				iter=vertexs.begin();
				for(int t=0;t<vertexIndexs[k];t++)
				{
					iter++;
				}
				MS3DVertex* mvt=(*iter);
				//将当前遍历到的顶点的坐标送入缓冲
				vertexCoordingBuffer0[num_vc++]=mvt->getCurrPosition()->getVector3fArray()[0];
				vertexCoordingBuffer0[num_vc++]=mvt->getCurrPosition()->getVector3fArray()[1];
				vertexCoordingBuffer0[num_vc++]=mvt->getCurrPosition()->getVector3fArray()[2];
			}
		}
		//设置当前组的纹理坐标缓冲
		texCoordingBuffer.push_back(texCoordingBuffer0);
		//设置当前组的顶点坐标缓冲
		vertexCoordingBuffer.push_back(vertexCoordingBuffer0);
	}
}
