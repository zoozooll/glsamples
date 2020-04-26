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
	//���ز�����Ϣ
	name = FileUtil::myReadString(32);//��ȡ���ʵ�����
	//��ȡ��������Ϣ
	ambient_color=new float[4];
	for(int j=0; j<4; j++){
		ambient_color[j] = FileUtil::myReadFloat();
	}
	//��ȡɢ�����Ϣ
	diffuse_color=new float[4];
	for(int j=0; j<4; j++){
		diffuse_color[j] =  FileUtil::myReadFloat();
	}
	//��ȡ�������Ϣ
	specular_color=new float[4];
	for(int j=0; j<4; j++){
		specular_color[j] = FileUtil::myReadFloat();
	}
	//��ȡ�Է�����Ϣ
	emissive_color=new float[4];
	for(int j=0; j<4; j++){
		emissive_color[j] = FileUtil::myReadFloat();
	}
	shininess =FileUtil::myReadFloat();//��ȡ�ֲڶ���Ϣ
	transparency =FileUtil::myReadFloat();//��ȡ͸������Ϣ
	FileUtil::myReadByte();//mode ��ʱ���ã������ӵ�
	//��ȡ����ͼƬ����
	string tn=FileUtil::myReadString(32)+
			FileUtil::myReadString(32)+
			FileUtil::myReadString(32)+
			FileUtil::myReadString(32);
	textureName =format(tn);
	//͸������ ��ʱ���ã������ӵ�--128
	FileUtil::myReadString(32);
	FileUtil::myReadString(32);
	FileUtil::myReadString(32);
	FileUtil::myReadString(32);

	//�������Ҳ���Ǽ�������ͼ��
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

//���ļ�·����ժȡ������ͼ���ļ������硰xx.jpg��
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
