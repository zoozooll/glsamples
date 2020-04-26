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

//关节信息
class MS3DJoint{
private:
	Mat4* tmatrix; //变换矩阵（当前绝对矩阵）

	vector<MS3DKeyFrameRotate*> rotates; //关键帧旋转值
	vector<MS3DKeyFramePosition*> positions; //关键帧位置坐标

	int count_rot;//关节旋转的关键帧数量
	int count_pos;//关节平移的关键帧数量

public:
	string name; //关节名称
	string parentName; //父关节名称
	bool ifparent;//是否存在父关节
	Vector3f* trotate; //初始旋转值
	Vector3f* tposition; //初始位置
	Mat4* relative; //相对矩阵（子关节在父关节坐标系中的变换矩阵）
	Mat4* absolute; //初始绝对矩阵（子关节在世界坐标系中的初始变换矩阵）

	MS3DJoint* parent; //父关节

	MS3DJoint();
	~MS3DJoint();

	void update(float ttime);//更新此关节
	Mat4* ttrotate(float time);//根据当前播放时间进行旋转插值计算
	Vector3f* ttposition(float time);//根据当前播放时间进行平移插值计算
	Mat4* getMatrix();
	Mat4* getAbsolute();

};

#endif /* MS3DJOINT_H_ */
