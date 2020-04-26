package com.bn.Sample7_7;
import javax.vecmath.Vector3f;
import com.bulletphysics.BulletGlobals;
import com.bulletphysics.collision.shapes.CapsuleShape;
import com.bulletphysics.collision.shapes.CapsuleShapeX;
import com.bulletphysics.collision.shapes.CollisionShape;
import com.bulletphysics.dynamics.DynamicsWorld;
import com.bulletphysics.dynamics.RigidBody;
import com.bulletphysics.dynamics.RigidBodyConstructionInfo;
import com.bulletphysics.dynamics.constraintsolver.Generic6DofConstraint;
import com.bulletphysics.linearmath.DefaultMotionState;
import com.bulletphysics.linearmath.Transform;
public class Doll {

	MySurfaceView mv;//场景渲染类对象的引用
	DynamicsWorld dynamicsWorld;//物理世界引用
	//碰撞形状数组，组成人偶身体各部分的胶囊形状均存储于此数组中
	CollisionShape[] bodyShapes= new CollisionShape[BodyPartIndex.BODYPART_COUNT.ordinal()];
	//刚体数组，组成人偶身体各部分的刚体均存储于此数组中，每个刚体对应一个胶囊形状
	RigidBody[] rigidBodies = new RigidBody[BodyPartIndex.BODYPART_COUNT.ordinal()];//刚体数组
	LoadedObjectVertexNormal[] bodyForDraws;//用于渲染身体各部分的加载物体
	//========属性值===========
	float mass=1;//刚体的质量
	//=========位置==========
	float bodyCenterH=7;//整个人偶中心的高度
	//===================胶囊的尺寸=============
	//头部
	float headR=0.5f;//头部胶囊两端半球的半径
	float headH=1.25f-2*headR;//头部胶囊中间圆柱的长度
	float headTH=1.25f;//头部胶囊总长度
	//脊椎
	float spineR=0.7f;//脊椎胶囊两端半球的半径
	float spineH=2.0f-2*spineR;//脊椎胶囊中间圆柱的长度
	float spineTH=2.0f;//脊椎胶囊总长度
	//右大臂
	float rightUpperArmR=0.3f;//右大臂胶囊两端半球的半径
	float rightUpperArmH=2.0f-2*rightUpperArmR;//右大臂胶囊中间圆柱的长度
	float rightUpperArmTH=2.0f;//右大臂胶囊总长度
	//右小臂
	float rightLowerArmR=0.3f;//右小臂胶囊两端半球的半径
	float rightLowerArmH=1.7f-2*rightLowerArmR;//右小臂胶囊中间圆柱的长度
	float rightLowerArmTH=1.7f;//右小臂胶囊总长度
	//左大臂
	float leftUpperArmR=0.3f;//左大臂胶囊两端半球的半径
	float leftUpperArmH=2.0f-2*leftUpperArmR;//左大臂胶囊中间圆柱的长度
	float leftUpperArmTH=2.0f;//左大臂胶囊总长度
	//左小臂
	float leftLowerArmR=0.3f;//左小臂胶囊两端半球的半径
	float leftLowerArmH=1.7f-2*leftLowerArmR;//左小臂胶囊中间圆柱的长度
	float leftLowerArmTH=1.7f;//左小臂胶囊总长度
	//骨盆
	float pelvisR=0.8f;//骨盆胶囊两端半球的半径
	float pelvisH=2.5f-pelvisR*2;//骨盆胶囊中间圆柱的长度
	float pelvisTH=2.5f;//骨盆胶囊总长度
	//右大腿
	float rightUpperLegR=0.3f;//右大腿胶囊两端半球的半径
	float rightUpperLegH=1.7f-rightUpperLegR*2;//右大腿胶囊中间圆柱的长度
	float rightUpperLegTH=1.7f;//右大腿胶囊总长度
	//左大腿
	float leftUpperLegR=0.3f;//左大腿胶囊两端半球的半径
	float leftUpperLegH=1.7f-leftUpperLegR*2;//左大腿胶囊中间圆柱的长度
	float leftUpperLegTH=1.7f;//左大腿胶囊总长度
	//左小腿
	float leftLowerLegR=0.3f;//左小腿胶囊两端半球的半径
	float leftLowerLegH=2.0f-2*leftLowerLegR;//左小腿胶囊中间圆柱的长度
	float leftLowerLegTH=2.0f;//左小腿胶囊总长度
	//右小腿
	float rightLowerLegR=0.3f;//右小腿胶囊两端半球的半径
	float rightLowerLegH=2.0f-2*rightLowerLegR;//右小腿胶囊中间圆柱的长度
	float rightLowerLegTH=2.0f;//右小腿胶囊总长度
	
	public Doll(MySurfaceView mv,DynamicsWorld dynamicsWorld,LoadedObjectVertexNormal[] bodyForDraws){
		this.mv=mv;//保存场景渲染类对象的引用
		this.dynamicsWorld=dynamicsWorld;//保存物理世界的引用
		this.bodyForDraws=bodyForDraws;//保存用于渲染类的物体列表引用
		initShapes();//初始化碰撞形状
		initRigidBodys();//初始化刚体
	}
	
	public void initShapes(){//初始化碰撞形状的方法
		//创建头部对应的胶囊碰撞形状
		bodyShapes[BodyPartIndex.BODYPART_HEAD.ordinal()] = new CapsuleShape(headR,headH);
		//创建脊椎对应的胶囊碰撞形状
		bodyShapes[BodyPartIndex.BODYPART_SPINE.ordinal()] = new CapsuleShape(spineR,spineH);
		bodyShapes[BodyPartIndex.BODYPART_RIGHT_UPPER_ARM.ordinal()] = new CapsuleShapeX(rightUpperArmR,rightUpperArmH);
		bodyShapes[BodyPartIndex.BODYPART_RIGHT_LOWER_ARM.ordinal()] = new CapsuleShapeX(rightLowerArmR,rightLowerArmH);
		bodyShapes[BodyPartIndex.BODYPART_LEFT_UPPER_ARM.ordinal()] = new CapsuleShapeX(leftUpperArmR,leftUpperArmH);
		bodyShapes[BodyPartIndex.BODYPART_LEFT_LOWER_ARM.ordinal()] = new CapsuleShapeX(leftLowerArmR,leftLowerArmH);
		bodyShapes[BodyPartIndex.BODYPART_PELVIS.ordinal()] = new CapsuleShape(pelvisR,pelvisH);
		bodyShapes[BodyPartIndex.BODYPART_RIGHT_UPPER_LEG.ordinal()] = new CapsuleShape(rightUpperLegR,rightUpperLegH);
		bodyShapes[BodyPartIndex.BODYPART_LEFT_UPPER_LEG.ordinal()] = new CapsuleShape(leftUpperLegR,leftUpperLegH);
		bodyShapes[BodyPartIndex.BODYPART_LEFT_LOWER_LEG.ordinal()] = new CapsuleShape(leftLowerLegR,leftLowerLegH);
		bodyShapes[BodyPartIndex.BODYPART_RIGHT_LOWER_LEG.ordinal()] = new CapsuleShape(rightLowerLegR,rightLowerLegH);
	}
	
	public void initRigidBodys(){//初始化刚体的方法
		//====================创建刚体=========================
		//头
		Transform tempTrans = new Transform();//创建刚体的初始变换对象
		tempTrans.setIdentity();//初始化变换对象
		tempTrans.origin.set(0,bodyCenterH+spineTH+headTH/2,0);//设置头部沿y轴进行平移
		//创建头部刚体
		rigidBodies[BodyPartIndex.BODYPART_HEAD.ordinal()] = addRigidBody(mass, tempTrans, bodyShapes[BodyPartIndex.BODYPART_HEAD.ordinal()]);
		//初始化变换对象
		tempTrans.setIdentity();
		tempTrans.origin.set(new Vector3f(0,bodyCenterH+spineTH/2,0));//设置脊椎沿y轴进行平移
		//创建脊椎刚体
		rigidBodies[BodyPartIndex.BODYPART_SPINE.ordinal()] = addRigidBody(mass, tempTrans, bodyShapes[BodyPartIndex.BODYPART_SPINE.ordinal()]);
		//右大臂
		tempTrans.setIdentity();
		tempTrans.origin.set(new Vector3f(spineR+rightUpperArmTH/2,bodyCenterH+spineTH-spineR,0));
		rigidBodies[BodyPartIndex.BODYPART_RIGHT_UPPER_ARM.ordinal()] = addRigidBody(mass, tempTrans, bodyShapes[BodyPartIndex.BODYPART_RIGHT_UPPER_ARM.ordinal()]);
		//右小臂
		tempTrans.setIdentity();
		tempTrans.origin.set(new Vector3f(spineR+rightUpperArmTH+rightLowerArmTH/2,bodyCenterH+spineTH-spineR,0));
		rigidBodies[BodyPartIndex.BODYPART_RIGHT_LOWER_ARM.ordinal()] = addRigidBody(mass, tempTrans, bodyShapes[BodyPartIndex.BODYPART_RIGHT_LOWER_ARM.ordinal()]);
		//左大臂
		tempTrans.setIdentity();
		tempTrans.origin.set(new Vector3f(-spineR-leftUpperArmTH/2,bodyCenterH+spineTH-spineR,0));
		rigidBodies[BodyPartIndex.BODYPART_LEFT_UPPER_ARM.ordinal()] = addRigidBody(mass, tempTrans, bodyShapes[BodyPartIndex.BODYPART_LEFT_UPPER_ARM.ordinal()]);
		//左小臂
		tempTrans.setIdentity();
		tempTrans.origin.set(new Vector3f(-spineR-leftUpperArmTH-leftLowerArmTH/2,bodyCenterH+spineTH-spineR,0));
		rigidBodies[BodyPartIndex.BODYPART_LEFT_LOWER_ARM.ordinal()] = addRigidBody(mass, tempTrans, bodyShapes[BodyPartIndex.BODYPART_LEFT_LOWER_ARM.ordinal()]);
		//骨盆
		tempTrans.setIdentity();
		tempTrans.origin.set(new Vector3f(0,bodyCenterH-pelvisTH/2,0));
		rigidBodies[BodyPartIndex.BODYPART_PELVIS.ordinal()] = addRigidBody(mass, tempTrans, bodyShapes[BodyPartIndex.BODYPART_PELVIS.ordinal()]);
		//右大腿
		tempTrans.setIdentity();
		tempTrans.origin.set(new Vector3f(pelvisR+rightUpperLegR,bodyCenterH-pelvisTH-rightUpperLegH/2+pelvisR,0));
		rigidBodies[BodyPartIndex.BODYPART_RIGHT_UPPER_LEG.ordinal()] = addRigidBody(mass, tempTrans, bodyShapes[BodyPartIndex.BODYPART_RIGHT_UPPER_LEG.ordinal()]);
		//左大腿
		tempTrans.setIdentity();
		tempTrans.origin.set(new Vector3f(-pelvisR-leftUpperLegR,bodyCenterH-pelvisTH-rightUpperLegH/2+pelvisR,0));
		rigidBodies[BodyPartIndex.BODYPART_LEFT_UPPER_LEG.ordinal()] = addRigidBody(mass, tempTrans, bodyShapes[BodyPartIndex.BODYPART_LEFT_UPPER_LEG.ordinal()]);
		//左小腿
		tempTrans.setIdentity();
		tempTrans.origin.set(new Vector3f(-pelvisR-leftUpperLegR,bodyCenterH-pelvisTH-leftUpperLegTH-leftLowerLegTH/2+pelvisR,0));
		rigidBodies[BodyPartIndex.BODYPART_LEFT_LOWER_LEG.ordinal()] = addRigidBody(mass, tempTrans, bodyShapes[BodyPartIndex.BODYPART_LEFT_LOWER_LEG.ordinal()]);
		//右小腿
		tempTrans.setIdentity();
		tempTrans.origin.set(new Vector3f(pelvisR+rightUpperLegR,bodyCenterH-pelvisTH-rightUpperLegTH-rightLowerLegTH/2+pelvisR,0));
		rigidBodies[BodyPartIndex.BODYPART_RIGHT_LOWER_LEG.ordinal()] = addRigidBody(mass, tempTrans, bodyShapes[BodyPartIndex.BODYPART_RIGHT_LOWER_LEG.ordinal()]);
		//============添加约束=============
		Generic6DofConstraint joint6DOF;//声明六自由度关节引用
		Transform localA = new Transform(); //创建变换对象A（表示从变换到刚体A质心的变换）
		Transform localB = new Transform();//创建变换对象B（表示从变换到刚体B质心的变换）
		//头部和脊椎约束
		localA.setIdentity();//初始化变换对象A
		localB.setIdentity();//初始化变换对象B
		localA.origin.set(0f, -headTH/2, 0f);//设置变换对象A的平移部分
		localB.origin.set(0,spineTH/2,0);//设置变换对象B的平移部分
		//创建头部和脊椎之间的六自由度约束
		joint6DOF = new Generic6DofConstraint(rigidBodies[BodyPartIndex.BODYPART_HEAD.ordinal()], rigidBodies[BodyPartIndex.BODYPART_SPINE.ordinal()], localA, localB, true);
		Vector3f limitTrans = new Vector3f();//用于存储六自由度关节旋转角度上下限的向量
		//设置旋转角度下限的3个分量
		limitTrans.set(-BulletGlobals.SIMD_PI * 0.1f, -BulletGlobals.FLT_EPSILON, -BulletGlobals.SIMD_PI * 0.1f);
		//设置关节转动角度的下限
		joint6DOF.setAngularLowerLimit(limitTrans);
		//设置旋转角度上限的3个分量
		limitTrans.set(BulletGlobals.SIMD_PI * 0.2f, BulletGlobals.FLT_EPSILON, BulletGlobals.SIMD_PI * 0.2f);
		//设置关节转动角度的下限
		joint6DOF.setAngularUpperLimit(limitTrans);
		//将约束添加进物理世界
		dynamicsWorld.addConstraint(joint6DOF, true);
		//右大臂和脊椎
		localA.setIdentity();
		localB.setIdentity();
		localA.origin.set(spineR,spineTH/2-rightUpperArmR*2, 0f);
		localB.origin.set(-rightUpperArmTH/2,0,0);
		joint6DOF = new Generic6DofConstraint(rigidBodies[BodyPartIndex.BODYPART_SPINE.ordinal()], rigidBodies[BodyPartIndex.BODYPART_RIGHT_UPPER_ARM.ordinal()], localA, localB, true);
		limitTrans.set(-BulletGlobals.FLT_EPSILON,-BulletGlobals.SIMD_PI * 0.1f, -BulletGlobals.SIMD_PI * 0.4f);
		joint6DOF.setAngularLowerLimit(limitTrans);
		limitTrans.set(BulletGlobals.FLT_EPSILON,BulletGlobals.SIMD_PI * 0.1f,  BulletGlobals.SIMD_PI * 0.4f);
		joint6DOF.setAngularUpperLimit(limitTrans);
		dynamicsWorld.addConstraint(joint6DOF, true);
		//右大臂和右小臂
		localA.setIdentity();
		localB.setIdentity();
		localA.origin.set(rightUpperArmTH/2,0, 0f);
		localB.origin.set(-rightLowerArmTH/2,0,0);
		joint6DOF = new Generic6DofConstraint(rigidBodies[BodyPartIndex.BODYPART_RIGHT_UPPER_ARM.ordinal()], rigidBodies[BodyPartIndex.BODYPART_RIGHT_LOWER_ARM.ordinal()], localA, localB, true);
		limitTrans.set(-BulletGlobals.FLT_EPSILON,-BulletGlobals.SIMD_PI * 0.4f,  -BulletGlobals.SIMD_PI * 0.05f);
		joint6DOF.setAngularLowerLimit(limitTrans);
		limitTrans.set(BulletGlobals.FLT_EPSILON,BulletGlobals.SIMD_PI * 0.4f,  BulletGlobals.SIMD_PI * 0.05f);
		joint6DOF.setAngularUpperLimit(limitTrans);
		dynamicsWorld.addConstraint(joint6DOF, true);
		//左大臂和脊椎
		localA.setIdentity();
		localB.setIdentity();
		localA.origin.set(-spineR,spineTH/2-leftUpperArmR*2, 0f);
		localB.origin.set(leftUpperArmTH/2,0,0);
		joint6DOF = new Generic6DofConstraint(rigidBodies[BodyPartIndex.BODYPART_SPINE.ordinal()], rigidBodies[BodyPartIndex.BODYPART_LEFT_UPPER_ARM.ordinal()], localA, localB, true);
		limitTrans.set( -BulletGlobals.FLT_EPSILON,-BulletGlobals.SIMD_PI * 0.1f, -BulletGlobals.SIMD_PI * 0.4f);
		joint6DOF.setAngularLowerLimit(limitTrans);
		limitTrans.set( BulletGlobals.FLT_EPSILON,BulletGlobals.SIMD_PI * 0.1f, BulletGlobals.SIMD_PI * 0.4f);
		joint6DOF.setAngularUpperLimit(limitTrans);
		dynamicsWorld.addConstraint(joint6DOF, true);
		//左小臂和左大臂
		localA.setIdentity();
		localB.setIdentity();
		localA.origin.set(-leftUpperArmTH/2,0, 0f);
		localB.origin.set(leftLowerArmTH/2,0,0);
		joint6DOF = new Generic6DofConstraint(rigidBodies[BodyPartIndex.BODYPART_LEFT_UPPER_ARM.ordinal()], rigidBodies[BodyPartIndex.BODYPART_LEFT_LOWER_ARM.ordinal()], localA, localB, true);
		limitTrans.set(-BulletGlobals.FLT_EPSILON,-BulletGlobals.SIMD_PI * 0.4f, -BulletGlobals.SIMD_PI * 0.05f);
		joint6DOF.setAngularLowerLimit(limitTrans);
		limitTrans.set(BulletGlobals.FLT_EPSILON,BulletGlobals.SIMD_PI * 0.4f,  BulletGlobals.SIMD_PI * 0.05f);
		joint6DOF.setAngularUpperLimit(limitTrans);
		dynamicsWorld.addConstraint(joint6DOF, true);
		//脊椎和骨盆
		localA.setIdentity();
		localB.setIdentity();
		localA.origin.set(0,-spineTH/2,0f);
		localB.origin.set(0,pelvisTH/2,0);
		joint6DOF = new Generic6DofConstraint(rigidBodies[BodyPartIndex.BODYPART_SPINE.ordinal()], rigidBodies[BodyPartIndex.BODYPART_PELVIS.ordinal()], localA, localB, true);
		limitTrans.set(-BulletGlobals.SIMD_PI * 0.2f, -BulletGlobals.SIMD_PI/2, -BulletGlobals.SIMD_PI * 0.2f);
		joint6DOF.setAngularLowerLimit(limitTrans);
		limitTrans.set(BulletGlobals.SIMD_PI * 0.2f, BulletGlobals.SIMD_PI/2, BulletGlobals.SIMD_PI * 0.2f);
		joint6DOF.setAngularUpperLimit(limitTrans);
		dynamicsWorld.addConstraint(joint6DOF, true);
		//骨盆和右大腿
		localA.setIdentity();
		localB.setIdentity();
		localA.origin.set(pelvisR+rightUpperLegR,-pelvisH/2,0f);
		localB.origin.set(0,rightUpperLegH/2,0);
		joint6DOF = new Generic6DofConstraint(rigidBodies[BodyPartIndex.BODYPART_PELVIS.ordinal()], rigidBodies[BodyPartIndex.BODYPART_RIGHT_UPPER_LEG.ordinal()], localA, localB, true);
		limitTrans.set(-BulletGlobals.SIMD_PI * 0.1f, -BulletGlobals.SIMD_PI * 0.1f, -BulletGlobals.SIMD_PI * 0.2f);
		joint6DOF.setAngularLowerLimit(limitTrans);
		limitTrans.set(BulletGlobals.SIMD_PI * 0.1f, BulletGlobals.SIMD_PI * 0.1f, BulletGlobals.SIMD_PI * 0.2f);
		joint6DOF.setAngularUpperLimit(limitTrans);
		dynamicsWorld.addConstraint(joint6DOF, true);
		//盆骨和左大腿
		localA.setIdentity();
		localB.setIdentity();
		localA.origin.set(-pelvisR-leftUpperLegR,-pelvisH/2,0f);
		localB.origin.set(0,rightUpperLegH/2,0);
		joint6DOF = new Generic6DofConstraint(rigidBodies[BodyPartIndex.BODYPART_PELVIS.ordinal()], rigidBodies[BodyPartIndex.BODYPART_LEFT_UPPER_LEG.ordinal()], localA, localB, true);
		limitTrans.set(-BulletGlobals.SIMD_PI * 0.1f, -BulletGlobals.SIMD_PI * 0.1f, -BulletGlobals.SIMD_PI * 0.2f);
		joint6DOF.setAngularLowerLimit(limitTrans);
		limitTrans.set(BulletGlobals.SIMD_PI * 0.1f, BulletGlobals.SIMD_PI * 0.1f, BulletGlobals.SIMD_PI * 0.2f);
		joint6DOF.setAngularUpperLimit(limitTrans);
		dynamicsWorld.addConstraint(joint6DOF, true);
		//左大腿和左小腿
		localA.setIdentity();
		localB.setIdentity();
		localA.origin.set(0,-leftUpperLegTH/2,0f);
		localB.origin.set(0,leftLowerLegTH/2,0);
		joint6DOF = new Generic6DofConstraint(rigidBodies[BodyPartIndex.BODYPART_LEFT_UPPER_LEG.ordinal()], rigidBodies[BodyPartIndex.BODYPART_LEFT_LOWER_LEG.ordinal()], localA, localB, true);
		limitTrans.set(-BulletGlobals.SIMD_PI * 0.5f, -BulletGlobals.FLT_EPSILON, -BulletGlobals.FLT_EPSILON);
		joint6DOF.setAngularLowerLimit(limitTrans);
		limitTrans.set(BulletGlobals.FLT_EPSILON, BulletGlobals.FLT_EPSILON, BulletGlobals.FLT_EPSILON);
		joint6DOF.setAngularUpperLimit(limitTrans);
		dynamicsWorld.addConstraint(joint6DOF, true);
		//右大腿和右小腿
		localA.setIdentity();
		localB.setIdentity();
		localA.origin.set(0,-rightUpperLegTH/2,0f);
		localB.origin.set(0,rightLowerLegTH/2,0);
		joint6DOF = new Generic6DofConstraint(rigidBodies[BodyPartIndex.BODYPART_RIGHT_UPPER_LEG.ordinal()], rigidBodies[BodyPartIndex.BODYPART_RIGHT_LOWER_LEG.ordinal()], localA, localB, true);
		limitTrans.set(-BulletGlobals.SIMD_PI * 0.5f, -BulletGlobals.FLT_EPSILON, -BulletGlobals.FLT_EPSILON);
		joint6DOF.setAngularLowerLimit(limitTrans);
		limitTrans.set(BulletGlobals.FLT_EPSILON, BulletGlobals.FLT_EPSILON, BulletGlobals.FLT_EPSILON);
		joint6DOF.setAngularUpperLimit(limitTrans);
		dynamicsWorld.addConstraint(joint6DOF, true);
	}
	private RigidBody addRigidBody(float mass, Transform startTransform, CollisionShape shape) {
		boolean isDynamic = (mass != 0f);
		Vector3f localInertia = new Vector3f();
		localInertia.set(0f, 0f, 0f);
		if (isDynamic) {
			shape.calculateLocalInertia(mass, localInertia);
		}
		DefaultMotionState myMotionState = new DefaultMotionState(startTransform);
		RigidBodyConstructionInfo rbInfo = new RigidBodyConstructionInfo(mass, myMotionState, shape, localInertia);
		rbInfo.additionalDamping = true;
		RigidBody body = new RigidBody(rbInfo);
		body.setRestitution(0.0f);
		body.setFriction(0.8f);
		dynamicsWorld.addRigidBody(body);
		return body;
	}
	
	public void drawSelf(int checkedIndex){
		MatrixState.pushMatrix();//保护现场
		for(int i=0;i<bodyForDraws.length;i++){//遍历用于渲染身体各部分的加载物体数组
			LoadedObjectVertexNormal lovo = bodyForDraws[i];
			if(i==checkedIndex){//如果该物体(身体部分)被拾取
				lovo.changeColor(true);//设置改变颜色的标志位为true
				lovo.drawSelf(rigidBodies[i]);//绘制该物体(身体部分)
			}else{//若该物体(身体部分)没有被拾取
				lovo.changeColor(false);//设置改变颜色的标志位为false
				lovo.drawSelf(rigidBodies[i]);//绘制该物体(身体部分)
			}
		}
		MatrixState.popMatrix();//恢复现场
	}
}