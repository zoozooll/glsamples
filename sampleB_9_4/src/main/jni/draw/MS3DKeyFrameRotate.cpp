/*
 * MS3DKeyFrameRotate.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DKeyFrameRotate.h"
#include "../myEncapsulation/FileUtil.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

MS3DKeyFrameRotate::MS3DKeyFrameRotate()
{
	time =FileUtil::myReadFloat();//读取关键帧时间
	rotate = new Vector4f();//读取关键帧旋转数据
	//将读取到的欧拉角形式的旋转数据转化为四元数形式（这是为了便于在关键帧之间进行插值计算）
	rotate->setFromEulerAngleToQuaternion(
			FileUtil::myReadFloat(),
			FileUtil::myReadFloat(),
			FileUtil::myReadFloat()
	);
}

MS3DKeyFrameRotate::~MS3DKeyFrameRotate()
{
	delete rotate;
}

float MS3DKeyFrameRotate::getTime()
{
	return time;
}

Vector4f* MS3DKeyFrameRotate::getRotate()
{
	return rotate;
}
