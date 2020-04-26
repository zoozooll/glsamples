/*
 * MS3DKeyFramePosition.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */

#ifndef MS3DKEYFRAMEPOSITION_H_
#define MS3DKEYFRAMEPOSITION_H_

#include "../element/Vector3f.h"

//�ؽ�ƽ�ƹؼ�֡��Ϣ
class MS3DKeyFramePosition{
private:
	float time; // ʱ��(��λΪ��)
	Vector3f* position; // λ������

public:
	MS3DKeyFramePosition();
	~MS3DKeyFramePosition();

	float getTime();
	Vector3f* getPosition();

};

#endif /* MS3DKEYFRAMEPOSITION_H_ */
