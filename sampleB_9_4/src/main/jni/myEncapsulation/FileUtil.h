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

	static unsigned char* binaryData;//����
	static int binaryData_index;//��������
	static unsigned char* loadBinary(std::string fname);//��ȡ�������ļ�
	static int myReadInt();//��һ��int���͵���,��ȡһ��32λ����������
	static short myReadUnsignedShort();//��һ��short���͵���,��ȡһ��16λ�޷�������������
	static int myReadByte();//��ȡһ���ֽ�
	static float myReadFloat();//��ȡһ��������
	static string myReadString(int length);//��string,��ȡ�ַ���

};

#endif
