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

//此类对象用来封装三角形组装的索引信息
class MS3DTriangle{
private:
	int* indexs;		 //组装索引值
	Vector3f* s;			 //三个顶点的纹理S坐标
	Vector3f* t;			 //三个顶点的纹理T坐标
	int smoothingGroup; //平滑组
	int groupIndex;	 //组索引

public:
	vector<Vector3f*> normals;//3个顶点法线向量

	MS3DTriangle();
	~MS3DTriangle();

	int* getIndexs();
	Vector3f* getS();
	Vector3f* getT();
	int getSmoothingGroup();
	int getGroupIndex();

};

#endif /* MS3DTRIANGLE_H_ */
