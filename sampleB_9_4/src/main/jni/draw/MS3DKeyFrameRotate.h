/*
 * MS3DKeyFrameRotate.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */

#ifndef MS3DKEYFRAMEROTATE_H_
#define MS3DKEYFRAMEROTATE_H_

#include "../element/Vector4f.h"

//�ؽ���ת�ؼ�֡��Ϣ
class MS3DKeyFrameRotate{
private:
	float time;			//ʱ��(��λΪ��)
	Vector4f* rotate;	//��ת����

public:
	MS3DKeyFrameRotate();
	~MS3DKeyFrameRotate();

	float getTime();
	Vector4f* getRotate();

};

#endif /* MS3DKEYFRAMEROTATE_H_ */
