#ifndef __FileUtil_H__//�����ظ�����
#define __FileUtil_H__

#include "android/asset_manager.h"//������Ҫ���ͷ�ļ�
#include "android/asset_manager_jni.h"
#include <string>

using namespace std;
class FileUtil
{
  public:
	static AAssetManager* aam;//ָ��AAssetManager�����ָ��
	static void setAAssetManager(AAssetManager* aamIn);//����AAssetManager����ָ��
	static string loadShaderStr(string fname);//������ɫ��
};

#endif
