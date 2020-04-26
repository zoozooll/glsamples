package com.bn.Sample7_8;

import java.nio.FloatBuffer;

import javax.vecmath.Quat4f;

import com.bn.Sample7_8.util.LoadedObjectVertexNormalTexture;
import com.bn.Sample7_8.util.MatrixState;
import com.bn.Sample7_8.util.SYSUtil;
import com.bulletphysics.dynamics.RigidBody;
import com.bulletphysics.dynamics.vehicle.RaycastVehicle;
import com.bulletphysics.linearmath.Transform;

public class VehicleDraw {
	RaycastVehicle vehicle;//交通工具对象引用
	LoadedObjectVertexNormalTexture vBox;
	LoadedObjectVertexNormalTexture vWheel;
	int vehicleTextureId;
	int mProgram;//自定义渲染管线着色器程序id
    int muMVPMatrixHandle;//总变换矩阵引用id
    int maPositionHandle; //顶点位置属性引用id  
    int maColorHandle; //顶点纹理坐标属性引用id 
    
    FloatBuffer   mVertexBuffer;//顶点坐标数据缓冲
	FloatBuffer   mColorBuffer;//顶点纹理坐标数据缓冲	
	int vCount;//顶点个数	
	
	public VehicleDraw(
			RaycastVehicle vehicle,//需要绘制的交通工具
			LoadedObjectVertexNormalTexture vBox,//从Obj加载的车身模型 
			LoadedObjectVertexNormalTexture vWheel, //从Obj加载的车轮模型
			int vehicleTextureId)//汽车纹理Id
	{
		this.vehicle = vehicle;//初始化待绘制交通工具引用
		this.vBox = vBox;//初始化车身模型引用
		this.vWheel = vWheel;//初始化车轮模型引用
		this.vehicleTextureId = vehicleTextureId;//汽车纹理Id
	}
	
	//绘制方法
	void drawSelf(){//绘制汽车的方法
		RigidBody vBody = vehicle.getRigidBody();			//得到当前汽车的刚体
		MatrixState.pushMatrix();							//保护现场
		//获得汽车当前的平移数据
		Transform trans = vBody.getMotionState().getWorldTransform(new Transform());
		//进行平移变换
		MatrixState.translate(trans.origin.x,trans.origin.y, trans.origin.z);
		//System.out.println("////"+trans.origin.y);
		//获取当前变换的旋转数据
		Quat4f ro=trans.getRotation(new Quat4f());			
		if(ro.x!=0||ro.y!=0||ro.z!=0){
			float[] fa=SYSUtil.fromSYStoAXYZ(ro);			//将四元数转换成AXYZ的形式
			MatrixState.rotate(fa[0], fa[1], fa[2], fa[3]);			//进行旋转变换
		}
		//绘制车身刚体
		vBox.drawSelf(vehicleTextureId);//绘制车身
		MatrixState.popMatrix();							//恢复现场
		for (int i = 0; i < vehicle.getNumWheels(); i++) {//遍历车轮
			vehicle.updateWheelTransform(i, true);			//更新车轮的变换
			MatrixState.pushMatrix();//保护现场
			trans = vehicle.getWheelInfo(i).worldTransform;	//获取车轮当前变换的旋转数据
			//进行平移变换
			MatrixState.translate(trans.origin.x,trans.origin.y,trans.origin.z);
			ro=trans.getRotation(new Quat4f());				//获取当前变换的旋转信息
			if(ro.x!=0||ro.y!=0||ro.z!=0){
				float[] fa=SYSUtil.fromSYStoAXYZ(ro);		//将四元数转换成AXYZ的形式
				MatrixState.rotate(fa[0], fa[1], fa[2], fa[3]);		//进行旋转变换
			}
			vWheel.drawSelf(vehicleTextureId);   			//绘制轮子
			MatrixState.popMatrix();						//恢复现场
	}}}