/*
 * MS3DTriangle.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DTriangle.h"
#include "../myEncapsulation/FileUtil.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

//此类对象用来封装三角形组装的索引信息
MS3DTriangle::MS3DTriangle()
{
	FileUtil::myReadUnsignedShort();//标志-暂时无用，读了扔掉
	//加载索引
	indexs=new int[3];
	indexs[0]=FileUtil::myReadUnsignedShort();
	indexs[1]=FileUtil::myReadUnsignedShort();
	indexs[2]=FileUtil::myReadUnsignedShort();
	//加载三个顶点的法向量
	for(int j=0; j<3; j++)
	{
		Vector3f* vt=new Vector3f(
			FileUtil::myReadFloat(),
			FileUtil::myReadFloat(),
			FileUtil::myReadFloat()
		);
		normals.push_back(vt);
	}
	//加载三个顶点的纹理S坐标
	s=new Vector3f(
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat()
	);
	//加载三个顶点的纹理T坐标
	t=new Vector3f(
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat()
	);
	smoothingGroup =FileUtil::myReadByte();//加载平滑组信息
	groupIndex =FileUtil::myReadByte();//加载组信息

}

MS3DTriangle::~MS3DTriangle()
{
	delete indexs;
	delete s;
	delete t;

	//删除所有Vector3f对象
	vector<Vector3f*>::iterator iter=normals.begin();
	for(;iter!=normals.end();iter++)
	{
		Vector3f* vt=(*iter);
		delete vt;
	}
	normals.clear();

}

int* MS3DTriangle::getIndexs()
{
	return indexs;
}

Vector3f* MS3DTriangle::getS()
{
	return s;
}

Vector3f* MS3DTriangle::getT()
{
	return t;
}

int MS3DTriangle::getSmoothingGroup()
{
	return smoothingGroup;
}

int MS3DTriangle::getGroupIndex()
{
	return groupIndex;
}
