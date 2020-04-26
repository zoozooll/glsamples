#include "FileUtil.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

AAssetManager* FileUtil::aam;
unsigned char* FileUtil::binaryData;
int FileUtil::binaryData_index = 0;

void FileUtil::setAAssetManager(AAssetManager* aamIn)
{
	aam=aamIn;
}

std::string FileUtil::loadShaderStr(std::string fname)//��sh�ű��м���shader���ݵķ���
{
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);
	off_t fileSize = AAsset_getLength(asset);
	unsigned char* data = (unsigned char*) malloc(fileSize + 1);
	data[fileSize] = '\0';
	int size = AAsset_read(asset, (void*)data, fileSize);
	std::string resultStr((const char*)data);
	return resultStr;
}

unsigned char* FileUtil::loadBinary(std::string fname){
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);
	off_t fileSize = AAsset_getLength(asset);
	unsigned char* data = (unsigned char*) malloc(fileSize + 1);
	data[fileSize] = '\0';
	int size = AAsset_read(asset, (void*)data, fileSize);
	return data;
}

int FileUtil::myReadInt(){//��һ��int���͵���,��ȡһ��32λ����������
	int readInt = 0;
	unsigned char * readInt_char = (unsigned char*)&readInt;
	for(int i = 0; i < sizeof(int); i++){
		readInt_char[i] = (unsigned char)binaryData[binaryData_index];
		binaryData_index++;
	}
//	LOGI("===========myReadString======readInt=%d",readInt);
	return readInt;
}

short FileUtil::myReadUnsignedShort(){//��һ��short���͵���,��ȡһ��16λ�޷�������������
	short readShort = 0;
	unsigned char * readShort_char = (unsigned char*)&readShort;
	for(int i = 0; i < sizeof(short); i++){
		readShort_char[i] = (unsigned char)binaryData[binaryData_index];
		binaryData_index++;
	}
	return readShort;
}

int FileUtil::myReadByte()//��ȡһ���ֽ�
{
	int readByte=0;
	unsigned char * readByte_char = (unsigned char*)&readByte;
	for(int i = 0; i < 1; i++){
		readByte_char[i] = (unsigned char)binaryData[binaryData_index];
		binaryData_index++;
	}
	return readByte;
}

float FileUtil::myReadFloat(){//��ȡһ��������
	float readFloat = 0.0f;
	unsigned char * readFloat_char = (unsigned char*)&readFloat;
	for(int i = 0; i < sizeof(float); i++){
		readFloat_char[i] = (unsigned char)binaryData[binaryData_index];
		binaryData_index++;
	}
	return readFloat;
}

string FileUtil::myReadString(int length){//��ȡ�ַ���
	int len = length;//myReadInt();
	if(len == 0){
		string s;
		return s;
	}
//	char c = 'a';
	unsigned char* ansChar = new unsigned char[len+1];
	for(int i = 0; i < len; i++){
		ansChar[i] = (unsigned char)binaryData[binaryData_index];
		binaryData_index++;
	}
	ansChar[len] = '\0';
	string str((const char*)ansChar);
//	LOGI("===========myReadString======str=%s",str.c_str());
	return str;
}
