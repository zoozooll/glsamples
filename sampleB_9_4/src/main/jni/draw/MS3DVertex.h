/*
 * MS3DVertex.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */

#ifndef MS3DVERTEX_H_
#define MS3DVERTEX_H_

#include "../element/Vector3f.h"

//顶点信息类
class MS3DVertex{
private:
	Vector3f* initPosition;	//从文件中读取的顶点原始xyz坐标
	Vector3f* currPosition;	//动画中实时变化的顶点xyz坐标
	int bone;			//骨骼ID

public:
	MS3DVertex();
	~MS3DVertex();

	Vector3f* getInitPosition();
	int getBone();
	Vector3f* getCurrPosition();
	void setCurrPosition(Vector3f* buffer);

};



#endif /* MS3DVERTEX_H_ */
