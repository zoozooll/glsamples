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
	time =FileUtil::myReadFloat();//��ȡ�ؼ�֡ʱ��
	rotate = new Vector4f();//��ȡ�ؼ�֡��ת����
	//����ȡ����ŷ������ʽ����ת����ת��Ϊ��Ԫ����ʽ������Ϊ�˱����ڹؼ�֮֡����в�ֵ���㣩
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
