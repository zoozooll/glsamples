/*
 * MS3DKeyFramePosition.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */

#ifndef MS3DKEYFRAMEPOSITION_H_
#define MS3DKEYFRAMEPOSITION_H_

#include "../element/Vector3f.h"

//关节平移关键帧信息
class MS3DKeyFramePosition{
private:
	float time; // 时间(单位为秒)
	Vector3f* position; // 位置坐标

public:
	MS3DKeyFramePosition();
	~MS3DKeyFramePosition();

	float getTime();
	Vector3f* getPosition();

};

#endif /* MS3DKEYFRAMEPOSITION_H_ */
