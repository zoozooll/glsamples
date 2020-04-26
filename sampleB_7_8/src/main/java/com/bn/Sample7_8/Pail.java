package com.bn.Sample7_8;

import javax.vecmath.AxisAngle4f;
import javax.vecmath.Quat4f;
import javax.vecmath.Vector3f;

import com.bn.Sample7_8.util.LoadedObjectVertexNormalTexture;
import com.bn.Sample7_8.util.MatrixState;
import com.bn.Sample7_8.util.SYSUtil;
import com.bulletphysics.collision.shapes.CollisionShape;
import com.bulletphysics.dynamics.DynamicsWorld;
import com.bulletphysics.dynamics.RigidBody;
import com.bulletphysics.dynamics.RigidBodyConstructionInfo;
import com.bulletphysics.linearmath.DefaultMotionState;
import com.bulletphysics.linearmath.Transform;

//路障类

public class Pail {
	//路障所在坐标
	float rx;
	float ry;
	float rz;
	float v_x =1.9f;//2.0f; //车宽
    float v_y = 1.9f;//1.0f;//车高
    float v_z = 1.9f;//5.0f;//车长
	DynamicsWorld dynamicsWorld;	//物理世界
	LoadedObjectVertexNormalTexture rbobj;//路障模型
	int vehicleTextureId;//纹理id
	RigidBody rb;
	float []f = new float[4];
	
	public Pail(float rx, float ry, float rz, float rbangle,DynamicsWorld dynamicsWorld,
			LoadedObjectVertexNormalTexture rbobj, int vehicleTextureId,CollisionShape rbShape) {
		this.rx = rx;
		this.ry = ry;
		this.rz = rz;

		this.dynamicsWorld = dynamicsWorld;
		this.rbobj = rbobj;
		this.vehicleTextureId = vehicleTextureId;

		Transform rbTransform = new Transform();	//创建刚体的初始变换对象
		rbTransform.setIdentity();					//对初始变换对象初始化
		rbTransform.origin.set(rx,ry,rz);			//移动刚体
		//四元数旋转
		f[2] = 1;
		if(rbangle!=0){
		 Quat4f q = new Quat4f(0,0,0,1f);				 //新建四元数
		 AxisAngle4f aa = new AxisAngle4f(0, 1, 0, rbangle); //按照路障要转的角度给四元数赋值
		 q.set(aa);
		 f = SYSUtil.fromSYStoAXYZ(q);				 //把四元数变为用数组表示	
		 rbTransform.setRotation(q);                  	 //对初始变换对象赋值
		}
		rb = localCreateRigidBody(100, rbTransform, rbShape);//获得刚体
	} 
	
	public RigidBody localCreateRigidBody(float mass, 
			Transform startTransform, CollisionShape shape) {
			boolean isDynamic = (mass != 0f);					//物体是否可以移动
			Vector3f localInertia = new Vector3f(0f, 0f, 0f);
			if (isDynamic) {									//如果物体可移动
				shape.calculateLocalInertia(mass, localInertia);		//计算惯性
			}
			//创建刚体的运动状态对象
			DefaultMotionState myMotionState = new DefaultMotionState(startTransform);
			//创建刚体描述信息对象
			RigidBodyConstructionInfo cInfo = 
			new RigidBodyConstructionInfo(mass, myMotionState, shape, localInertia);
			RigidBody body = new RigidBody(cInfo);				//新建刚体对象
			//设置摩擦系数
			body.setFriction(0.8f);
			body.setRestitution(0.6f);//设置反弹系数
			dynamicsWorld.addRigidBody(body);				//把此刚体加入到物理世界中
			return body;									//返回刚体
			}
			public void drawSelf(){		
				MatrixState.pushMatrix();							//保护现场
				//获得汽车当前的移动矩阵
				Transform trans = rb.getMotionState().getWorldTransform(new Transform());
				//进行移位变换
				MatrixState.translate(trans.origin.x,trans.origin.y, trans.origin.z);
				//System.out.println("////"+trans.origin.y);
				Quat4f ro=trans.getRotation(new Quat4f());			//获取当前变换的旋转信息
				if(ro.x!=0||ro.y!=0||ro.z!=0){
					float[] fa=SYSUtil.fromSYStoAXYZ(ro);			//将四元数转换成AXYZ的形式
					MatrixState.rotate(fa[0], fa[1], fa[2], fa[3]);			//执行旋转
				}
				//绘制车身刚体
				rbobj.drawSelf(vehicleTextureId);
				MatrixState.popMatrix();							//恢复现场
									//恢复现场
			}
			public void clientResetScene() {
				Transform tr = new Transform();
				tr.setIdentity();
				tr.origin.set(this.rx,this.ry,this.rz);

				{
					Quat4f q = new Quat4f(0,0,0,1f);
					AxisAngle4f aa = new AxisAngle4f(0, 1, 0, 0);
					q.set(aa);
					tr.setRotation(q);
				}
				rb.setCenterOfMassTransform(tr); //设置质量中心
//				rb.setLinearVelocity(new Vector3f(0, 0, 0));//设置线速度
//				rb.setAngularVelocity(new Vector3f(0, 0, 0));//设置角速度
				dynamicsWorld.getBroadphase().getOverlappingPairCache().cleanProxyFromPairs(rb.getBroadphaseHandle(), dynamicsWorld.getDispatcher());
			}

}