/*
 * MS3DVertex.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DVertex.h"
#include "../myEncapsulation/FileUtil.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

MS3DVertex::MS3DVertex()
{

	FileUtil::myReadByte();  //标志--暂时无用，读了扔掉
	initPosition =new Vector3f(
			FileUtil::myReadFloat(),
			FileUtil::myReadFloat(),
			FileUtil::myReadFloat()
	);//顶点XYZ坐标
	bone = FileUtil::myReadByte();//骨骼ID
	FileUtil::myReadByte();  //标志--暂时无用，读了扔掉

}

MS3DVertex::~MS3DVertex()
{
	delete initPosition;
	delete currPosition;
}

Vector3f* MS3DVertex::getInitPosition()
{
	return initPosition;
}

int MS3DVertex::getBone()
{
	return bone;
}

Vector3f* MS3DVertex::getCurrPosition()
{
	return currPosition;
}

void MS3DVertex::setCurrPosition(Vector3f* buffer)
{
	currPosition = buffer;
}
