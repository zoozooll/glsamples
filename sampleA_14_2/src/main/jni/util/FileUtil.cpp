#include "FileUtil.h"

AAssetManager* FileUtil::aam;

void FileUtil::setAAssetManager(AAssetManager* aamIn)//��ʼ��AAssetManager����
{
	aam=aamIn;//��AAssetManager����ֵ
}

string FileUtil::loadShaderStr(string fname)//������ɫ���ļ�
{
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);//����AAsset����
	off_t fileSize = AAsset_getLength(asset);//��ȡAAsset����ĳ���
	unsigned char* data = (unsigned char*) malloc(fileSize + 1);//�����ڴ�
	data[fileSize] = '\0';//��ʼ��
	int size = AAsset_read(asset, (void*)data, fileSize);//��ȡ�ļ���С
	std::string resultStr((const char*)data);//��ý���ַ���

	return resultStr;
}
