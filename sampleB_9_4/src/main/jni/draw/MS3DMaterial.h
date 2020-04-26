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

//������Ϣ
class MS3DMaterial{
private:
	string name;		//��������
	static string format(string path);//���ļ�·����ժȡ������ͼ���ļ������硰xx.jpg��

public:
	MS3DMaterial(JNIEnv * env,jobject obj);
	~MS3DMaterial();

	float* ambient_color;	//������
	float* diffuse_color;	//ɢ���
	float* specular_color;	//�����
	float* emissive_color;	//�Է���
	float shininess;		//�ֲڶ� 0-128
	float transparency;		//͸���� 0-1
	string textureName;		//�����ļ�����

	string getName();
};

#endif /* MS3DMATERIAL_H_ */
