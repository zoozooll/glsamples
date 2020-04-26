#ifndef __FileUtil_H__
#define __FileUtil_H__
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"

#include <string>

using namespace std;

class FileUtil
{
  public:
	static AAssetManager* aam;
	static void setAAssetManager(AAssetManager* aamIn);
	static std::string loadShaderStr(std::string fname);

	static unsigned char* binaryData;//数据
	static int binaryData_index;//数据索引
	static unsigned char* loadBinary(std::string fname);//读取二进制文件
	static int myReadInt();//读一个int类型的数,读取一个32位整数并返回
	static short myReadUnsignedShort();//读一个short类型的数,读取一个16位无符号整数并返回
	static int myReadByte();//读取一个字节
	static float myReadFloat();//读取一个浮点数
	static string myReadString(int length);//读string,读取字符串

};

#endif
