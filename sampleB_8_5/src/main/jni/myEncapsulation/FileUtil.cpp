#include "FileUtil.h"

AAssetManager* FileUtil::aam;

void FileUtil::setAAssetManager(AAssetManager* aamIn)
{
	aam=aamIn;
}


std::string FileUtil::loadShaderStr(std::string fname)
{
	AAsset* asset =AAssetManager_open(aam,fname.c_str(),AASSET_MODE_UNKNOWN);
	off_t fileSize = AAsset_getLength(asset);
	unsigned char* data = (unsigned char*) malloc(fileSize + 1);
	data[fileSize] = '\0';
	int size = AAsset_read(asset, (void*)data, fileSize);
	std::string resultStr((const char*)data);

	return resultStr;
}
