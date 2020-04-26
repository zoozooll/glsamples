/*
 * MS3DModel.h
 *
 *  Created on: 2015-9-7
 *      Author: Administrator
 */

#ifndef MS3DMODEL_H_
#define MS3DMODEL_H_

#include <vector>
#include <map>
#include <jni.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "MS3DHeader.h"
#include "MS3DVertex.h"
#include "MS3DTriangle.h"
#include "MS3DGroup.h"
#include "MS3DMaterial.h"
#include "MS3DJoint.h"

using namespace std;

class MS3DModel{
private:
	void updateVectexs();//�����и��¶������ݵķ���
	void updateVectex(int index);//�����ض�����ķ���
	void initBuffer();//��ʼ������

public:
	int num_ver;//��������
	int num_gro;//������
	int num_joi;//�ؽ�����

	int textId;

	float fps;		//fps��Ϣ
	float current_time;	//��ǰʱ��
	float totalTime;	//��ʱ��
	int frame_count;	//�ؼ�֡��

	vector<GLfloat*> vertexCoordingBuffer;//�����������ݻ���
	vector<GLfloat*> texCoordingBuffer;	//�����������ݻ���
	MS3DHeader* header;				//ͷ��Ϣ
	vector<MS3DVertex*> vertexs;			//������Ϣ
	vector<MS3DTriangle*> triangles;		//����������
	vector<MS3DGroup*> groups;			//����Ϣ
	vector<MS3DMaterial*> materials;		//������Ϣ(Ŀǰ�õ�����Ҫ��������)
	vector<MS3DJoint*> joints;				//�ؽ���Ϣ
	static map<string,int> textureManager;//���������

	GLuint mProgram;//�Զ�����Ⱦ���߳���id
	GLuint muMVPMatrixHandle;//�ܱ任��������id
	GLuint maPositionHandle; //����λ����������id
	GLuint maTexCoorHandle; //��������������������id

	MS3DModel(JNIEnv* env,jobject obj);
	~MS3DModel();

	void initShader();//��ʼ����ɫ��
	void draw(bool isUpdate);//����ģ��
	void animate(float ttime);//���ж����ķ���
	void updateJoint(float ttime);//���¹ؽڵķ���
//	void ppt();
	GLfloat* updateVertexs(int triangleCount,int* indexs);

};



#endif /* MS3DMODEL_H_ */
