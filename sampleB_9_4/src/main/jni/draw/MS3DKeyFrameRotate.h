/*
 * MS3DKeyFrameRotate.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */

#ifndef MS3DKEYFRAMEROTATE_H_
#define MS3DKEYFRAMEROTATE_H_

#include "../element/Vector4f.h"

//关节旋转关键帧信息
class MS3DKeyFrameRotate{
private:
	float time;			//时间(单位为秒)
	Vector4f* rotate;	//旋转向量

public:
	MS3DKeyFrameRotate();
	~MS3DKeyFrameRotate();

	float getTime();
	Vector4f* getRotate();

};

#endif /* MS3DKEYFRAMEROTATE_H_ */
