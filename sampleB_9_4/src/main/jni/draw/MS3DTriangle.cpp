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

//�������������װ��������װ��������Ϣ
MS3DTriangle::MS3DTriangle()
{
	FileUtil::myReadUnsignedShort();//��־-��ʱ���ã������ӵ�
	//��������
	indexs=new int[3];
	indexs[0]=FileUtil::myReadUnsignedShort();
	indexs[1]=FileUtil::myReadUnsignedShort();
	indexs[2]=FileUtil::myReadUnsignedShort();
	//������������ķ�����
	for(int j=0; j<3; j++)
	{
		Vector3f* vt=new Vector3f(
			FileUtil::myReadFloat(),
			FileUtil::myReadFloat(),
			FileUtil::myReadFloat()
		);
		normals.push_back(vt);
	}
	//�����������������S����
	s=new Vector3f(
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat()
	);
	//�����������������T����
	t=new Vector3f(
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat()
	);
	smoothingGroup =FileUtil::myReadByte();//����ƽ������Ϣ
	groupIndex =FileUtil::myReadByte();//��������Ϣ

}

MS3DTriangle::~MS3DTriangle()
{
	delete indexs;
	delete s;
	delete t;

	//ɾ������Vector3f����
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
