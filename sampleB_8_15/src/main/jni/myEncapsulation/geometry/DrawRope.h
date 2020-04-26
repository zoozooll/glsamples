/*
 * DrawRope.h
 *
 *  Created on: 2015Äê2ÔÂ4ÈÕ
 *      Author: Nr
 */

#ifndef _DRAWROPE_H_
#define _DRAWROPE_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "CylinderSideInclined.h"
#include "Circle.h"

#include "../../Bullet/LinearMath/btVector3.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

class DrawRope {
	Circle* m_bottom_circle;
	Circle* m_top_circle;
	CylinderSideInclined* csi;
	int m_texId;
	int m_nums;
	btVector3* m_pos;
public:
	DrawRope(btVector3* pos, int nums, int texId);
	virtual ~DrawRope();
	void drawSelf();
};

#endif /* MYENCAPSULATION_DRAWROPE_H_ */
