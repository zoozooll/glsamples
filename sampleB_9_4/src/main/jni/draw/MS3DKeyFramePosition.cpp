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
	time = FileUtil::myReadFloat();// ��ȡ�ؼ�֡ʱ��
	// ��ȡ�ؼ�֡λ����Ϣ
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
