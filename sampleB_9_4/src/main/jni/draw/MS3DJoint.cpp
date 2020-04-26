/*
 * MS3DJoint.cpp
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */
#include "MS3DJoint.h"
#include "../myEncapsulation/FileUtil.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

MS3DJoint::MS3DJoint()
{
	FileUtil::myReadByte();//��־ ��ʱ���ã������ӵ�
	name = FileUtil::myReadString(32);//��ȡ�ؽ�����
	parentName = FileUtil::myReadString(32);//��ȡ���ؽ�����
	//���عؽڵ���ת����
	trotate = new Vector3f(
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat()
	);
	//���عؽڵ�λ������
	tposition = new Vector3f(
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat(),
		FileUtil::myReadFloat()
	);
	//��ȡ�ؽ���ת�Ĺؼ�֡����
	count_rot = FileUtil::myReadUnsignedShort();
	//��ȡ�ؽ�ƽ�ƵĹؼ�֡����
	count_pos = FileUtil::myReadUnsignedShort();
	//���ؽ���ת�Ĺؼ�֡������Ϊ0������عؽ���ת�Ĺؼ�֡��ֵ
	if(count_rot > 0)
	{
		for(int i=0; i<count_rot; i++)//ѭ���������е���ת�ؼ�֡��Ϣ
		{
			MS3DKeyFrameRotate* rotateKF = new MS3DKeyFrameRotate();
			rotates.push_back(rotateKF);
		}
	}
	//���ؽ�ƽ�ƵĹؼ�֡������Ϊ0������عؽ�ƽ�ƵĹؼ�֡��ֵ
	if(count_pos > 0)
	{
		//ѭ������ÿ��ƽ�ƹؼ�֡����Ϣ
		for(int i = 0; i < count_pos; i++)
		{
			MS3DKeyFramePosition* position =new MS3DKeyFramePosition();
			positions.push_back(position);
		}
	}
	ifparent=false;//�޸��ؽ�
	tmatrix =new Mat4();

}

MS3DJoint::~MS3DJoint()
{
	delete trotate;
	delete tposition;
	delete relative;
	delete absolute;
	delete tmatrix;
	delete parent;

	//ɾ������MS3DKeyFrameRotate����
	vector<MS3DKeyFrameRotate*>::iterator iter=rotates.begin();
	for(;iter!=rotates.end();iter++)
	{
		MS3DKeyFrameRotate* vt=(*iter);
		delete vt;
	}
	rotates.clear();
	//ɾ������MS3DKeyFramePosition����
	vector<MS3DKeyFramePosition*>::iterator iter0=positions.begin();
	for(;iter0!=positions.end();iter0++)
	{
		MS3DKeyFramePosition* vt=(*iter0);
		delete vt;
	}
	positions.clear();
}

void MS3DJoint::update(float ttime)
{//���´˹ؽ�
	//����ת��ƽ�ƵĹؽ��䵱ǰ���Ծ�����ڳ�ʼ���Ծ���
	if(rotates.size()<=0&&positions.size()<=0)
	{
		tmatrix->copyFrom(absolute);
		return;
	}
	Mat4* matrix0 = ttrotate(ttime);//����ת
	matrix0->setTranslation(ttposition(ttime));//��ƽ��
	matrix0->mul(relative, matrix0);//��������Ծ������
	if(ifparent)
	{//�Ƿ��и��ؽ�
		delete tmatrix;
		tmatrix = matrix0->mul(parent->tmatrix, matrix0);//�и��ؽ�
	}else{
		delete tmatrix;
		tmatrix = matrix0;//�޸��ؽ�
	}
}

Mat4* MS3DJoint::ttrotate(float time)
{//���ݵ�ǰ����ʱ�������ת��ֵ����
	Mat4* m =new Mat4();
	int index = 0;
	int size = count_rot;
	//����ʱ��ȷ����ǰ֡Ӧ������һ����ʼ�ؼ�֡�������
	vector<MS3DKeyFrameRotate*>::iterator iter=rotates.begin();
	while(index<size)
	{
		MS3DKeyFrameRotate* mf= (*iter);
		if(mf->getTime()<time)
		{
			iter++;
			index++;
		}else
		{
			break;
		}
	}
	if(index == 0){
		MS3DKeyFrameRotate* mf= (*iter);
		m->genRotateFromQuaternion(mf->getRotate());
	}else if(index==size){
		iter--;
		MS3DKeyFrameRotate* mf= (*iter);
		m->genRotateFromQuaternion(mf->getRotate());
	}else{
		MS3DKeyFrameRotate* right =(*iter);//�˹ؼ�֡����ת
		iter--;
		MS3DKeyFrameRotate* left =(*iter);//��һ�ؼ�֡����ת

		Vector4f* v =new Vector4f();//��ֵ������ǰ֡����ת
		v->interpolate(left->getRotate(), right->getRotate(),(time - left->getTime())/(right->getTime() - left->getTime()));
		//����Ԫ����ʽ����ת��Ϊ������ʽ
		m->genRotateFromQuaternion(v);

		delete v;
	}
	return m;
}

Vector3f* MS3DJoint::ttposition(float time)
{//���ݵ�ǰ����ʱ�����ƽ�Ʋ�ֵ����
	int index = 0;
	int size = count_pos;
	//����ʱ��ȷ����ǰ֡Ӧ������һ����ʼ�ؼ�֡�������
	vector<MS3DKeyFramePosition*>::iterator iter=positions.begin();
	while(index<size)
	{
		MS3DKeyFramePosition* mf=(*iter);
		if(mf->getTime()<time)
		{
			iter++;
			index++;
		}else
		{
			break;
		}
	}
	if(index == 0){
		MS3DKeyFramePosition* mf=(*iter);
		return mf->getPosition();
	}else if(index == size){
		iter--;
		MS3DKeyFramePosition* mf=(*iter);
		return mf->getPosition();
	}else{
		//�˹ؼ�֡��ƽ��
		MS3DKeyFramePosition* right =(*iter);
		//��һ�ؼ�֡��ƽ��
		iter--;
		MS3DKeyFramePosition* left =(*iter);

		//��ֵ�������ǰ�ؼ�֡��ƽ��
		Vector3f* v = new Vector3f();
		v->interpolate(left->getPosition(), right->getPosition(),
				(time - left->getTime())/ (right->getTime() - left->getTime()));
		return v;
	}
}

Mat4* MS3DJoint::getMatrix()
{
	return tmatrix;
}

Mat4* MS3DJoint::getAbsolute()
{
	return absolute;
}


