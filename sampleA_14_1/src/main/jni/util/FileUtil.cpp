#include "FileUtil.h"

AAssetManager* FileUtil::aam;//ָ��AAssetManager�����ָ��

void FileUtil::setAAssetManager(AAssetManager* aamIn)//����AAssetManager����ָ��
{
	aam=aamIn;//��AAssetManager����ָ�븳ֵ
}

string FileUtil::loadShaderStr(string fname)//������ɫ���ļ�
{
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);//����AAsset����
	off_t fileSize = AAsset_getLength(asset);//��ȡAAsset����ĳ���
	unsigned char* data = (unsigned char*) malloc(fileSize + 1);//�����ڴ�
	data[fileSize] = '\0';//�����һ���ַ�����Ϊ'\0'
	int size = AAsset_read(asset, (void*)data, fileSize);//��ȡ�ļ���С
	std::string resultStr((const char*)data);//��ý���ַ���

	return resultStr;
}
