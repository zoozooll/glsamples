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
	FileUtil::myReadByte();  //标志--暂时无用，读了扔掉
	FileUtil::myReadString(32);//读取组名称--暂时无用，读了扔掉
	count_ind =FileUtil::myReadUnsignedShort();//读取组内三角形数量
	//创建组内三角形索引数组
	indicies=new int[count_ind];
	for(int j=0; j<count_ind; j++){ //加载组内各个三角形的索引
		indicies[j] = FileUtil::myReadUnsignedShort();
	}
	materialIndex =FileUtil::myReadByte();//读取材质索引
}

MS3DGroup::~MS3DGroup()
{
	delete indicies;
}

int* MS3DGroup::getIndicies()
{
	return indicies;//组内的三角形的对应索引
}

int MS3DGroup::getMaterialIndex()
{
	return materialIndex;//材质索引
}

int MS3DGroup::getCount_ind()
{
	return count_ind;//组内三角形数量
}
