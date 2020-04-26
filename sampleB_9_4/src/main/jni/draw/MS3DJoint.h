/*
 * MS3DJoint.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */

#ifndef MS3DJOINT_H_
#define MS3DJOINT_H_

#include "../element/Vector3f.h"
#include "../element/Mat4.h"
#include "MS3DKeyFrameRotate.h"
#include "MS3DKeyFramePosition.h"
#include <string>
#include <vector>

using namespace std;

//�ؽ���Ϣ
class MS3DJoint{
private:
	Mat4* tmatrix; //�任���󣨵�ǰ���Ծ���

	vector<MS3DKeyFrameRotate*> rotates; //�ؼ�֡��תֵ
	vector<MS3DKeyFramePosition*> positions; //�ؼ�֡λ������

	int count_rot;//�ؽ���ת�Ĺؼ�֡����
	int count_pos;//�ؽ�ƽ�ƵĹؼ�֡����

public:
	string name; //�ؽ�����
	string parentName; //���ؽ�����
	bool ifparent;//�Ƿ���ڸ��ؽ�
	Vector3f* trotate; //��ʼ��תֵ
	Vector3f* tposition; //��ʼλ��
	Mat4* relative; //��Ծ����ӹؽ��ڸ��ؽ�����ϵ�еı任����
	Mat4* absolute; //��ʼ���Ծ����ӹؽ�����������ϵ�еĳ�ʼ�任����

	MS3DJoint* parent; //���ؽ�

	MS3DJoint();
	~MS3DJoint();

	void update(float ttime);//���´˹ؽ�
	Mat4* ttrotate(float time);//���ݵ�ǰ����ʱ�������ת��ֵ����
	Vector3f* ttposition(float time);//���ݵ�ǰ����ʱ�����ƽ�Ʋ�ֵ����
	Mat4* getMatrix();
	Mat4* getAbsolute();

};

#endif /* MS3DJOINT_H_ */
