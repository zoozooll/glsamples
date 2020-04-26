/*
 * MS3DMaterial.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include <stdio.h>
#include <jni.h>
#include "MS3DMaterial.h"
#include "MS3DModel.h"
#include "../myEncapsulation/FileUtil.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

using namespace std;

MS3DMaterial::MS3DMaterial(JNIEnv* env,jobject obj)
{
	//加载材质信息
	name = FileUtil::myReadString(32);//读取材质的名称
	//读取环境光信息
	ambient_color=new float[4];
	for(int j=0; j<4; j++){
		ambient_color[j] = FileUtil::myReadFloat();
	}
	//读取散射光信息
	diffuse_color=new float[4];
	for(int j=0; j<4; j++){
		diffuse_color[j] =  FileUtil::myReadFloat();
	}
	//读取镜面光信息
	specular_color=new float[4];
	for(int j=0; j<4; j++){
		specular_color[j] = FileUtil::myReadFloat();
	}
	//读取自发光信息
	emissive_color=new float[4];
	for(int j=0; j<4; j++){
		emissive_color[j] = FileUtil::myReadFloat();
	}
	shininess =FileUtil::myReadFloat();//读取粗糙度信息
	transparency =FileUtil::myReadFloat();//读取透明度信息
	FileUtil::myReadByte();//mode 暂时无用，读了扔掉
	//读取纹理图片名称
	string tn=FileUtil::myReadString(32)+
			FileUtil::myReadString(32)+
			FileUtil::myReadString(32)+
			FileUtil::myReadString(32);
	textureName =format(tn);
	//透明材质 暂时无用，读了扔掉--128
	FileUtil::myReadString(32);
	FileUtil::myReadString(32);
	FileUtil::myReadString(32);
	FileUtil::myReadString(32);

	//添加纹理（也就是加载纹理图）
	jclass cl = env->FindClass("com/bn/bullet/GL2JNIView");
	jmethodID tid = env->GetStaticMethodID(cl,"initTextureRepeat","(Landroid/opengl/GLSurfaceView;Ljava/lang/String;)I");
	jstring tname = env->NewStringUTF(textureName.c_str());
	int boxTexId = env->CallStaticIntMethod(cl,tid,obj,tname);
	MS3DModel::textureManager[name]=boxTexId;

}

MS3DMaterial::~MS3DMaterial()
{
	delete ambient_color;
	delete diffuse_color;
	delete specular_color;
	delete emissive_color;
}

//从文件路径中摘取出纹理图的文件名，如“xx.jpg”
string MS3DMaterial::format(string path)
{
	int offset = path.rfind("\\");
	int endset = path.rfind("g");
	if(offset!=string::npos&&endset!=string::npos)
	{
		return path.substr(offset+1,endset-1);
	}
	return path;
}

string MS3DMaterial::getName()
{
	return name;
}
