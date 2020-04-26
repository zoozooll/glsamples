/*
 * MS3DGroup.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DGroup.h"
#include "../myEncapsulation/FileUtil.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

MS3DGroup::MS3DGroup()
{
	FileUtil::myReadByte();  //��־--��ʱ���ã������ӵ�
	FileUtil::myReadString(32);//��ȡ������--��ʱ���ã������ӵ�
	count_ind =FileUtil::myReadUnsignedShort();//��ȡ��������������
	//����������������������
	indicies=new int[count_ind];
	for(int j=0; j<count_ind; j++){ //�������ڸ��������ε�����
		indicies[j] = FileUtil::myReadUnsignedShort();
	}
	materialIndex =FileUtil::myReadByte();//��ȡ��������
}

MS3DGroup::~MS3DGroup()
{
	delete indicies;
}

int* MS3DGroup::getIndicies()
{
	return indicies;//���ڵ������εĶ�Ӧ����
}

int MS3DGroup::getMaterialIndex()
{
	return materialIndex;//��������
}

int MS3DGroup::getCount_ind()
{
	return count_ind;//��������������
}
