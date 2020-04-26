/*
 * MS3DKeyFramePosition.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DKeyFramePosition.h"
#include "../myEncapsulation/FileUtil.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

MS3DKeyFramePosition::MS3DKeyFramePosition()
{
	time = FileUtil::myReadFloat();// 读取关键帧时间
	// 读取关键帧位置信息
	position =new Vector3f(
			FileUtil::myReadFloat(),
			FileUtil::myReadFloat(),
			FileUtil::myReadFloat()
	);
}

MS3DKeyFramePosition::~MS3DKeyFramePosition()
{
	delete position;
}

float MS3DKeyFramePosition::getTime()
{
	return time;
}
Vector3f* MS3DKeyFramePosition::getPosition()
{
	return position;
}
