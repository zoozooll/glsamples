/*
 * MS3DVertex.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */

#ifndef MS3DVERTEX_H_
#define MS3DVERTEX_H_

#include "../element/Vector3f.h"

//������Ϣ��
class MS3DVertex{
private:
	Vector3f* initPosition;	//���ļ��ж�ȡ�Ķ���ԭʼxyz����
	Vector3f* currPosition;	//������ʵʱ�仯�Ķ���xyz����
	int bone;			//����ID

public:
	MS3DVertex();
	~MS3DVertex();

	Vector3f* getInitPosition();
	int getBone();
	Vector3f* getCurrPosition();
	void setCurrPosition(Vector3f* buffer);

};



#endif /* MS3DVERTEX_H_ */
