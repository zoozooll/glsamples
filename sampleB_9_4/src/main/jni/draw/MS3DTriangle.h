/*
 * MS3DTriangle.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */

#ifndef MS3DTRIANGLE_H_
#define MS3DTRIANGLE_H_

#include "../element/Vector3f.h"
#include <vector>

using namespace std;

//�������������װ��������װ��������Ϣ
class MS3DTriangle{
private:
	int* indexs;		 //��װ����ֵ
	Vector3f* s;			 //�������������S����
	Vector3f* t;			 //�������������T����
	int smoothingGroup; //ƽ����
	int groupIndex;	 //������

public:
	vector<Vector3f*> normals;//3�����㷨������

	MS3DTriangle();
	~MS3DTriangle();

	int* getIndexs();
	Vector3f* getS();
	Vector3f* getT();
	int getSmoothingGroup();
	int getGroupIndex();

};

#endif /* MS3DTRIANGLE_H_ */
