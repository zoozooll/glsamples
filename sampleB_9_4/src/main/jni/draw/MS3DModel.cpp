/*
 * MS3DModel.cpp
 *
 *  Created on: 2015-9-7
 *      Author: Administrator
 */
#include "MS3DModel.h"
#include "../myEncapsulation/FileUtil.h"
#include "../myEncapsulation/ShaderUtil.h"
#include "myEncapsulation/MatrixState.h"
#include "../element/Mat4.h"
#include <string>
#include <map>

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

using namespace std;
map<string,int> MS3DModel::textureManager;
MS3DModel::MS3DModel(JNIEnv * env,jobject obj)
{
	initShader();//��ʼ����ɫ��

	//����ģ��
	header = new MS3DHeader();	//����ͷ��Ϣ

	num_ver=FileUtil::myReadUnsignedShort();//��ȡ��������
	for(int i=0; i<num_ver; i++){//ѭ����ȡÿ���������Ϣ
		MS3DVertex* vertex = new MS3DVertex();
		vertexs.push_back(vertex);
	}
	int num_tri=FileUtil::myReadUnsignedShort();//��ȡ����������
	for(int i=0; i<num_tri; i++)//ѭ������ÿһ�������ε���װ������Ϣ
	{
		MS3DTriangle* triangle= new MS3DTriangle();
		triangles.push_back(triangle);
	}
	//��������Ϣ
	num_gro=FileUtil::myReadUnsignedShort();//��ȡ������
	for(int i=0; i<num_gro; i++)//ѭ������ÿ�������Ϣ
	{
		MS3DGroup* group = new MS3DGroup();
		groups.push_back(group);
	}
	//���ز�����Ϣ
	int num_mat=FileUtil::myReadUnsignedShort();//��ȡ��������
	for(int i=0; i<num_mat; i++)//ѭ������ÿ�����ʵ���Ϣ
	{
		MS3DMaterial* mal = new MS3DMaterial(env,obj);
		materials.push_back(mal);
	}
	fps=FileUtil::myReadFloat();//����֡������Ϣ
	current_time = FileUtil::myReadFloat();//��ǰʱ��
	frame_count = FileUtil::myReadInt();//�ؼ�֡��
	totalTime = frame_count / fps;//���㶯����ʱ��
	//���عؽ���Ϣ
	num_joi=FileUtil::myReadUnsignedShort();//��ȡ�ؽ�����
	map<string, MS3DJoint*> mapp;
	for(int i = 0; i < num_joi; i++){//ѭ������ÿ���ؽڵ���Ϣ
		MS3DJoint* joint = new MS3DJoint();

		mapp[joint->name]=joint;//���ؽ���Ϣ����洢��map�Ա�����
		//�ж��Ƿ��и��ؽ�
		map<string,MS3DJoint*>::iterator iter=mapp.begin();
		for(; iter != mapp.end(); iter++)
		{
			string pname=iter->first;
			if(pname==(joint->parentName))
			{
				joint->parent = mapp[joint->parentName]; //��ô˹ؽڵĸ��ؽ�
				joint->ifparent=true;
				break;
			}
		}
		joint->relative =new Mat4();//������Ծ���
		joint->relative->loadIdentity();
		//������ת
		joint->relative->genRotationFromEulerAngle(joint->trotate);
		joint->relative->setTranslation(joint->tposition); //����ƽ��
		joint->absolute = new Mat4();//���þ��Ծ���
		joint->absolute->loadIdentity();
		if(joint->ifparent){//�Ƿ��и��ؽ�
			//�и��ؽڵĻ����Ծ�����ڸ��ؽڵľ��Ծ�������ӹؽڵ���Ծ���
			joint->absolute->mul(joint->parent->absolute,joint->relative);
		}else{
			//�޸��ؽڵĻ���Ծ���Ϊ���Ծ���
			joint->absolute->copyFrom(joint->relative);
		}

		joints.push_back(joint);
	}
	mapp.clear();//���map

	initBuffer();//��ʼ������
}

MS3DModel::~MS3DModel()
{
	//ɾ��MS3DHeader����
	delete header;

	//ɾ������MS3DVertex����
	vector<MS3DVertex*>::iterator iter=vertexs.begin();
	for(;iter!=vertexs.end();iter++)
	{
		MS3DVertex* vt=(*iter);
		delete vt;
	}
	vertexs.clear();

	//ɾ������MS3DTriangle����
	vector<MS3DTriangle*>::iterator iter0=triangles.begin();
	for(;iter0!=triangles.end();iter0++)
	{
		MS3DTriangle* vt=(*iter0);
		delete vt;
	}
	triangles.clear();

	//ɾ������MS3DGroup����
	vector<MS3DGroup*>::iterator iter1=groups.begin();
	for(;iter1!=groups.end();iter1++)
	{
		MS3DGroup* vt=(*iter1);
		delete vt;
	}
	groups.clear();

	//ɾ������MS3DMaterial����
	vector<MS3DMaterial*>::iterator iter2=materials.begin();
	for(;iter2!=materials.end();iter2++)
	{
		MS3DMaterial* vt=(*iter2);
		delete vt;
	}
	materials.clear();
	//ɾ������MS3DJoint����
	vector<MS3DJoint*>::iterator iter3=joints.begin();
	for(;iter3!=joints.end();iter3++)
	{
		MS3DJoint* vt=(*iter3);
		delete vt;
	}
	joints.clear();
}

//��ʼ����ɫ��
void MS3DModel::initShader()
{
	//���ض�����ɫ���Ľű�
	string strVertex = FileUtil::loadShaderStr("vertex.sh");
	const char* anyShapeVertex = strVertex.c_str();
	//����ƬԪ��ɫ���Ľű�
	string strFragment = FileUtil::loadShaderStr("frag.sh");
	const char* anyShapeFragment = strFragment.c_str();

	//���ڶ�����ɫ����ƬԪ��ɫ����������
	mProgram = ShaderUtil::createProgram(anyShapeVertex, anyShapeFragment);
	//��ȡ�������ܱ任��������id
	muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
	//����λ������id
	maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
	//��ȡ�����ж�����ɫ��������id
	maTexCoorHandle = glGetAttribLocation(mProgram, "aTexCoor");

}
//���Ʒ���
void MS3DModel::draw(bool isUpdate)
{
	glUseProgram(mProgram); //ָ��ʹ��ĳ��shader����
	MatrixState::copyMVMatrix();
	//�����ձ任������shader����
	glUniformMatrix4fv(muMVPMatrixHandle,1,0,MatrixState::getFinalMatrix());//�����ձ任������shader����
	//���ö�����������
	MS3DGroup* group;
	MS3DMaterial* material;   //����
	int* indexs;

	vector<MS3DGroup*>::iterator iter1=groups.begin();
	vector<GLfloat*>::iterator iter_vcb=vertexCoordingBuffer.begin();
	vector<GLfloat*>::iterator iter_tcb=texCoordingBuffer.begin();
	vector<MS3DMaterial*>::iterator iter2;

	for(;iter1!=groups.end();iter1++,iter_tcb++,iter_vcb++)
	{
		group =(*iter1);	//��ȡ��ǰ����Ϣ����
		indexs = group->getIndicies();//��ȡ���������ε���������
		int triangleCount  =group->getCount_ind();//��ȡ���������ε�����
		//�в��ʣ�������Ҫ��ָ��Ҫ������
		if(group->getMaterialIndex() > -1)
		{
			iter2=materials.begin();
			for(int t=0;t<(group->getMaterialIndex());t++)
			{
				iter2++;
			}
			material = (*iter2);
			//������
			textId=MS3DModel::textureManager[material->getName()];
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textId);

			//�����������ж���������������id,һ����2�������������ͣ�GL_FALSE��һ����ռ�ֽ���������������׵�ַ
			glVertexAttribPointer(maTexCoorHandle,2,GL_FLOAT,GL_FALSE,2*4,(*iter_tcb));
			//����������������
			glEnableVertexAttribArray(maTexCoorHandle);
		}
		if(isUpdate)
		{//���¶��㻺��
			(*iter_vcb)=updateVertexs(triangleCount, indexs);
		}
		//�����������ж���λ����������id,һ����3�������������ͣ�GL_FALSE��һ����ռ�ֽ������������ݵ��׵�ַ
		glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3*4, (*iter_vcb));
		glEnableVertexAttribArray(maPositionHandle);
		//�ö��㷨���л���
		glDrawArrays(GL_TRIANGLES, 0, triangleCount*3);

		glDisableVertexAttribArray(maPositionHandle);
		glDisableVertexAttribArray(maTexCoorHandle);

	}
}

//���ж����ķ���
void MS3DModel::animate(float ttime)
{
	if(current_time != ttime){//��ͬʱ�䲻������
		updateJoint(ttime);	//���¹ؽ�
		updateVectexs();	//���¶���
		draw(true);	//ִ�л���
	}else{
		//ִ�л���
		draw(false);
	}
}
//���¹ؽڵķ���
void MS3DModel::updateJoint(float ttime)
{
	current_time = ttime;//���µ�ǰʱ��
	if(current_time > totalTime){//ʱ�䳬����ʱ����Ϊ��
		current_time = 0.0f;
	}
	vector<MS3DJoint*>::iterator iter=joints.begin();
	for(;iter!=joints.end();iter++){//����ÿ���ؽ�
		MS3DJoint* vt=(*iter);
		vt->update(current_time);
	}

}
//�����и��¶������ݵķ���
void MS3DModel::updateVectexs()
{
	int count =num_ver;//��ȡ��������
	for(int i=0; i<count; i++){	//����ÿ������
		updateVectex(i);
	}
}
//�����ض�����ķ���
void MS3DModel::updateVectex(int index)
{
	//��ȡ��ǰ��Ҫ���µĶ����Ӧ�Ķ�����Ϣ����
	vector<MS3DVertex*>::iterator iter=vertexs.begin();
	for(int i=0;i<index;i++)
	{
		iter++;
	}
	MS3DVertex* vt = (*iter);
	//�Ƿ��йؽڹ���ID
	if(vt->getBone() == -1){//�޹ؽڿ���
		vt->setCurrPosition(vt->getInitPosition());
	}else{//�йؽڿ���
		vector<MS3DJoint*>::iterator iter3=joints.begin();
		int it=vt->getBone();
		for(int i=0;i<it;i++)
		{
			iter3++;
		}
		MS3DJoint* joint =(*iter3);//��ȡ��Ӧ�Ĺؽ�
		//���ݹؽڵ�ʵʱ�任�����������㾭�ؽ�Ӱ����λ��
		vt->setCurrPosition(joint->getMatrix()->transform(joint->getAbsolute()->invTransformAndRotate(vt->getInitPosition())));
	}
}
GLfloat* MS3DModel::updateVertexs(int triangleCount,int* indexs)
{
	MS3DTriangle* triangle;
	int* vertexIndexs;	//��������
	//���¶��㻺��
	GLfloat* buffers = new float[triangleCount*9];	//buffer����
	int num_v=0;

	vector<MS3DTriangle*>::iterator iter0;
	vector<MS3DVertex*>::iterator iter;
	for(int j=0; j<triangleCount; j++)
	{//�����ڵ�ÿ��������ѭ��
		iter0=triangles.begin();
		for(int t=0;t<indexs[j];t++)
		{
			iter0++;
		}
		triangle = (*iter0);//��ȡ��ǰҪ�������������Ϣ����
		vertexIndexs = triangle->getIndexs();	//��ȡ����������������Ķ�������
		//�������ε�������������ݷ��붥�����ݻ��壨ʵ������������ε���װ��

		for(int k=0; k<3; k++)
		{
			iter=vertexs.begin();
			for(int t=0;t<vertexIndexs[k];t++)
			{
				iter++;
			}

			MS3DVertex* mvt=(*iter);
			buffers[num_v++]=mvt->getCurrPosition()->getVector3fArray()[0];
			buffers[num_v++]=mvt->getCurrPosition()->getVector3fArray()[1];
			buffers[num_v++]=mvt->getCurrPosition()->getVector3fArray()[2];
		}
	}
	return buffers;
}
//��ʼ������
void MS3DModel::initBuffer()
{
	//���ؽڸ��µ���ʼʱ�䣨ʱ��Ϊ0��ʱ�䣩
	updateJoint(0.0f);
	updateVectexs();//���¶�������
	int triangleCount = 0;//ÿ�������θ���
	MS3DGroup* group;//��ʱ����Ϣ
	MS3DTriangle* triangle;//��ʱ��������Ϣ
	int* indexs;//����������
	int* vertexIndexs;//��������

	//��ģ���е�ÿ�������ѭ������
	vector<MS3DGroup*>::iterator iter1=groups.begin();
	for(;iter1!=groups.end();iter1++)
	{

		group = (*iter1);//��ȡ��ǰҪ�������
		indexs = group->getIndicies();  //��ȡ������������������
		triangleCount =group->getCount_ind();//��ȡ���������ε�����

		GLfloat* texCoordingBuffer0=new float[triangleCount*6];//���ݻ���
		GLfloat* vertexCoordingBuffer0=new float[triangleCount*9];

		vector<MS3DTriangle*>::iterator iter0;

		int num_tc=0;
		int num_vc=0;

		//ѭ�������ڵ�ÿ�������ν��д���
		for(int j=0; j<triangleCount; j++)
		{
			iter0=triangles.begin();
			for(int k=0;k<indexs[j];k++)
			{
				iter0++;

			}
			triangle = (*iter0);//��ȡ��ǰҪ�����������
			vertexIndexs = triangle->getIndexs();//��ȡ�������и������������
			vector<MS3DVertex*>::iterator iter;
			//���������е������������ѭ������
			for(int k=0; k<3; k++)
			{
				//����ǰ�������Ķ��������ST�������뻺��
				texCoordingBuffer0[num_tc++]=triangle->getS()->getVector3fArray()[k];
				texCoordingBuffer0[num_tc++]=triangle->getT()->getVector3fArray()[k];
				//��ȡ��ǰ��Ķ����������ݻ���
				iter=vertexs.begin();
				for(int t=0;t<vertexIndexs[k];t++)
				{
					iter++;
				}
				MS3DVertex* mvt=(*iter);
				//����ǰ�������Ķ�����������뻺��
				vertexCoordingBuffer0[num_vc++]=mvt->getCurrPosition()->getVector3fArray()[0];
				vertexCoordingBuffer0[num_vc++]=mvt->getCurrPosition()->getVector3fArray()[1];
				vertexCoordingBuffer0[num_vc++]=mvt->getCurrPosition()->getVector3fArray()[2];
			}
		}
		//���õ�ǰ����������껺��
		texCoordingBuffer.push_back(texCoordingBuffer0);
		//���õ�ǰ��Ķ������껺��
		vertexCoordingBuffer.push_back(vertexCoordingBuffer0);
	}
}
