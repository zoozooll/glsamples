/*
 * MS3DMaterial.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */

#ifndef MS3DMATERIAL_H_
#define MS3DMATERIAL_H_

#include <string>
#include <jni.h>

using namespace std;

//材质信息
class MS3DMaterial{
private:
	string name;		//材质名称
	static string format(string path);//从文件路径中摘取出纹理图的文件名，如“xx.jpg”

public:
	MS3DMaterial(JNIEnv * env,jobject obj);
	~MS3DMaterial();

	float* ambient_color;	//环境光
	float* diffuse_color;	//散射光
	float* specular_color;	//镜面光
	float* emissive_color;	//自发光
	float shininess;		//粗糙度 0-128
	float transparency;		//透明度 0-1
	string textureName;		//材质文件名称

	string getName();
};

#endif /* MS3DMATERIAL_H_ */
