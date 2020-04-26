package com.bn.Sample10_1;//声明包
public class IntersectantUtil {	
	/*
	 * 1、通过在屏幕上的触控位置，计算对应的近平面上坐标，
	 * 以便求出AB两点在摄像机坐标系中的坐标
	 * 2、将AB两点在摄像机中坐标系中的坐标乘以摄像机矩阵的逆矩阵，
	 * 以便求得AB两点在世界坐标系中的坐标
	 */
	public static float[] calculateABPosition(//计算AB点坐标的方法
		float x,float y,//触控点x、y坐标
		float w,float h,//屏幕宽度、高度
		//近平面参数
		float left,//视角left值
		float top,//视角top值
		float near,//视角near值
		float far//视角far值
	)
	{
		//将以屏幕左上角为原点的触控点坐标换算为以屏幕中心为坐标原点的坐标
		float x0=x-w/2;
		float y0=h/2-y;		
		//计算对应的near面上的x、y坐标
		float xNear=2*x0*left/w;
		float yNear=2*y0*top/h;
		//计算对应的far面上的x、y坐标
		float ratio=far/near;//计算far与near的比值
		float xFar=ratio*xNear;//计算对应的far面上的x坐标
		float yFar=ratio*yNear;//计算对应的far面上的y坐标
		//摄像机坐标系中A的坐标
        float ax=xNear;//摄像机坐标系中A点的x坐标
        float ay=yNear;//摄像机坐标系中A点的y坐标
        float az=-near;//摄像机坐标系中A点的z坐标
        //摄像机坐标系中B的坐标
        float bx=xFar;//摄像机坐标系中B点的x坐标
        float by=yFar;//摄像机坐标系中B点的y坐标
        float bz=-far; //摄像机坐标系中B点的z坐标
        //通过摄像机坐标系中A、B两点的坐标，求世界坐标系中A、B两点的坐标
		float[] A = MatrixState.fromPtoPreP(new float[] { ax, ay, az });//求世界坐标系中A点坐标
		float[] B = MatrixState.fromPtoPreP(new float[] { bx, by, bz });//求世界坐标系中B点坐标
		return new float[] {//返回AB两点在世界坐标系中的坐标
			A[0],A[1],A[2],
			B[0],B[1],B[2]
		};
	}
}