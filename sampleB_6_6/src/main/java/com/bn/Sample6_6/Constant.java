package com.bn.Sample6_6;

import java.nio.FloatBuffer;

public class Constant 
{
	static FloatBuffer mVertexBufferForFlag;//顶点缓冲引用
	static Object lockA=new Object();//锁对象A
	static Object lockB=new Object();//锁对象B
	final static int NUMROWS = 7;//旗面网格行数
	final static int NUMCOLS = 10;//旗面网格列数
	final static int NUMVERTICES = (NUMROWS+1)*(NUMCOLS+1);//顶点数目
	final static int NUMSPTINGS = (NUMROWS*(NUMCOLS+1)+(NUMROWS+1)*NUMCOLS+2*NUMROWS*NUMCOLS);//弹簧数目
	final static float RSTER = 0.75f/NUMROWS;//质点行间距
	final static float CSTER = 1.0f/NUMCOLS;//质点列间距
	final static float 	KRESTITUTION=0.3f; //反弹系数
	final static float COLLISIONTOLERANCE = -6.6f;//地面位置
	final static float 	FRICTIONFACTOR=0.9f; //摩擦系数
	final static float 	FLAGPOLERADIUS=0.04f;//旗杆半径
	final static float GRAVITY = -0.7f;//重力加速度
	final static float SPRING_TENSION_CONSTANT = 500.f;//构造弹簧参数
	final static float SPRING_SHEAR_CONSTANT = 300.f;//剪力弹簧参数
	final static float SPRING_DAMPING_CONSTANT = 2.f;//弹簧阻尼
	static boolean isC = false;//是否开启碰撞检测标志
	static float WindForce = 2.0f;//风力
	final static float DRAGCOEFFICIENT = 0.01f;//空气阻力
}