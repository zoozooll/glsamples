/*
 * MS3DJoint.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DJoint.h"
#include "../myEncapsulation/FileUtil.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

MS3DJoint::MS3DJoint()
{
	FileUtil::myReadByte();//标志 暂时无用，读了扔掉
	name = FileUtil::myReadString(32);//读取关节名称
	parentName = FileUtil::myReadString(32);//读取父关节名称
	//加载关节的旋转数据
	trotate = new Vector3f(
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat()
	);
	//加载关节的位置数据
	tposition = new Vector3f(
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat()
	);
	//读取关节旋转的关键帧数量
	count_rot = FileUtil::myReadUnsignedShort();
	//读取关节平移的关键帧数量
	count_pos = FileUtil::myReadUnsignedShort();
	//若关节旋转的关键帧数量不为0，则加载关节旋转的关键帧的值
	if(count_rot > 0)
	{
		for(int i=0; i<count_rot; i++)//循环加载所有的旋转关键帧信息
		{
			MS3DKeyFrameRotate* rotateKF = new MS3DKeyFrameRotate();
			rotates.push_back(rotateKF);
		}
	}
	//若关节平移的关键帧数量不为0，则加载关节平移的关键帧的值
	if(count_pos > 0)
	{
		//循环加载每个平移关键帧的信息
		for(int i = 0; i < count_pos; i++)
		{
			MS3DKeyFramePosition* position =new MS3DKeyFramePosition();
			positions.push_back(position);
		}
	}
	ifparent=false;//无父关节
	tmatrix =new Mat4();

}

MS3DJoint::~MS3DJoint()
{
	delete trotate;
	delete tposition;
	delete relative;
	delete absolute;
	delete tmatrix;
	delete parent;

	//删除所有MS3DKeyFrameRotate对象
	vector<MS3DKeyFrameRotate*>::iterator iter=rotates.begin();
	for(;iter!=rotates.end();iter++)
	{
		MS3DKeyFrameRotate* vt=(*iter);
		delete vt;
	}
	rotates.clear();
	//删除所有MS3DKeyFramePosition对象
	vector<MS3DKeyFramePosition*>::iterator iter0=positions.begin();
	for(;iter0!=positions.end();iter0++)
	{
		MS3DKeyFramePosition* vt=(*iter0);
		delete vt;
	}
	positions.clear();
}

void MS3DJoint::update(float ttime)
{//更新此关节
	//无旋转无平移的关节其当前绝对矩阵等于初始绝对矩阵
	if(rotates.size()<=0&&positions.size()<=0)
	{
		tmatrix->copyFrom(absolute);
		return;
	}
	Mat4* matrix0 = ttrotate(ttime);//先旋转
	matrix0->setTranslation(ttposition(ttime));//再平移
	matrix0->mul(relative, matrix0);//与自身相对矩阵相乘
	if(ifparent)
	{//是否有父关节
		delete tmatrix;
		tmatrix = matrix0->mul(parent->tmatrix, matrix0);//有父关节
	}else{
		delete tmatrix;
		tmatrix = matrix0;//无父关节
	}
}

Mat4* MS3DJoint::ttrotate(float time)
{//根据当前播放时间进行旋转插值计算
	Mat4* m =new Mat4();
	int index = 0;
	int size = count_rot;
	//根据时间确定当前帧应该由哪一个起始关键帧往后计算
	vector<MS3DKeyFrameRotate*>::iterator iter=rotates.begin();
	while(index<size)
	{
		MS3DKeyFrameRotate* mf= (*iter);
		if(mf->getTime()<time)
		{
			iter++;
			index++;
		}else
		{
			break;
		}
	}
	if(index == 0){
		MS3DKeyFrameRotate* mf= (*iter);
		m->genRotateFromQuaternion(mf->getRotate());
	}else if(index==size){
		iter--;
		MS3DKeyFrameRotate* mf= (*iter);
		m->genRotateFromQuaternion(mf->getRotate());
	}else{
		MS3DKeyFrameRotate* right =(*iter);//此关键帧的旋转
		iter--;
		MS3DKeyFrameRotate* left =(*iter);//上一关键帧的旋转

		Vector4f* v =new Vector4f();//插值产生当前帧的旋转
		v->interpolate(left->getRotate(), right->getRotate(),(time - left->getTime())/(right->getTime() - left->getTime()));
		//将四元数形式的旋转变为矩阵形式
		m->genRotateFromQuaternion(v);

		delete v;
	}
	return m;
}

Vector3f* MS3DJoint::ttposition(float time)
{//根据当前播放时间进行平移插值计算
	int index = 0;
	int size = count_pos;
	//根据时间确定当前帧应该由哪一个起始关键帧往后计算
	vector<MS3DKeyFramePosition*>::iterator iter=positions.begin();
	while(index<size)
	{
		MS3DKeyFramePosition* mf=(*iter);
		if(mf->getTime()<time)
		{
			iter++;
			index++;
		}else
		{
			break;
		}
	}
	if(index == 0){
		MS3DKeyFramePosition* mf=(*iter);
		return mf->getPosition();
	}else if(index == size){
		iter--;
		MS3DKeyFramePosition* mf=(*iter);
		return mf->getPosition();
	}else{
		//此关键帧的平移
		MS3DKeyFramePosition* right =(*iter);
		//上一关键帧的平移
		iter--;
		MS3DKeyFramePosition* left =(*iter);

		//插值计算出当前关键帧的平移
		Vector3f* v = new Vector3f();
		v->interpolate(left->getPosition(), right->getPosition(),
				(time - left->getTime())/ (right->getTime() - left->getTime()));
		return v;
	}
}

Mat4* MS3DJoint::getMatrix()
{
	return tmatrix;
}

Mat4* MS3DJoint::getAbsolute()
{
	return absolute;
}


