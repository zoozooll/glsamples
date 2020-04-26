/*
 * MS3DHeader.cpp
 *
 *  Created on: 2015-9-4
 *      Author: Administrator
 */
#include "MS3DHeader.h"
#include "../myEncapsulation/FileUtil.h"
#include <string>

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

using namespace std;

MS3DHeader::MS3DHeader()
{
	id = FileUtil::myReadString(10);//初始化固定的标志字符串
	version = FileUtil::myReadInt();//初始化版本号
}
MS3DHeader::~MS3DHeader()
{
}


string MS3DHeader::getId()
{
	return id;
}

int MS3DHeader::getVersion()
{
	return version;
}

