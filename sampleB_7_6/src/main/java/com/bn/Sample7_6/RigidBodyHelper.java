package com.bn.Sample7_6;

import javax.vecmath.Vector3f;
import com.bulletphysics.collision.shapes.CollisionShape;
import com.bulletphysics.dynamics.DiscreteDynamicsWorld;
import com.bulletphysics.dynamics.RigidBody;
import com.bulletphysics.dynamics.RigidBodyConstructionInfo;
import com.bulletphysics.linearmath.DefaultMotionState;
import com.bulletphysics.linearmath.Transform;

public class RigidBodyHelper {
	
	RigidBody body;
	boolean noGravity;
  
	 public RigidBodyHelper(CollisionShape shape,float mass,DiscreteDynamicsWorld dynamicsWorld,
			 float restitution,float friction,Vector3f origin,boolean noGravity){
		 	this.noGravity=noGravity;
	    	boolean isDynamic = (mass!=0);
			Vector3f localInertia = new Vector3f(0,0,0);
			if(isDynamic){
				shape.calculateLocalInertia(mass, localInertia);
			}
	    	//创建刚体的初始变换对象
			Transform groundTransform = new Transform();
			groundTransform.setIdentity();
			groundTransform.origin.set(origin);		
			//创建刚体的运动状态对象
			DefaultMotionState myMotionState = new DefaultMotionState(groundTransform);
			//创建刚体信息对象
			RigidBodyConstructionInfo rbInfo = new RigidBodyConstructionInfo(mass, myMotionState, shape, localInertia);
			//创建刚体
			body = new RigidBody(rbInfo);
			//设置反弹系数
			body.setRestitution(restitution);
			//设置摩擦系数
			body.setFriction(friction);
			
			//将刚体添加进物理世界
			//bullet是在add刚体的时候为刚体设置其重力的
			dynamicsWorld.addRigidBody(body);
			//一定要在add刚体之后再从新设置重力才能将原重力覆盖
			if(noGravity){
		 		body.setGravity(new Vector3f(0,0,0));
		 	}
	    } 
}