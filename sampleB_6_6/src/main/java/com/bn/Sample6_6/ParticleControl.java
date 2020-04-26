package com.bn.Sample6_6;

import static com.bn.Sample6_6.Constant.*;

import com.bn.util.Vector3f;


public class ParticleControl {
	Particle particles[][] = new Particle[NUMROWS+1][NUMCOLS+1];//粒子数组
	Spring springs[] = new Spring[NUMSPTINGS];//弹簧数组
    float vertices[]=new float[NUMCOLS*NUMROWS*2*3*3];//每个顶点xyz三个坐标
    Collision collisions[] = new Collision[NUMVERTICES*2];//碰撞体数组
    Vector3f temp = new Vector3f(0, 0, 0);//临时向量对象1
    Vector3f temp2 = new Vector3f(0, 0, 0);//临时向量对象2
	public ParticleControl()
	{
		initalize();
	}
	public float[] getVerties()//获取一帧数据
	{
	    int count=0;//顶点计数器
        for(int r=0;r<NUMROWS;r++)
        {
        	for(int c=0;c<NUMCOLS;c++)
        	{        		
        		vertices[count++]=particles[r][c].pvPosition.x;
        		vertices[count++]=particles[r][c].pvPosition.y;
        		vertices[count++]=particles[r][c].pvPosition.z;
        		
        		vertices[count++]=particles[r+1][c].pvPosition.x;
        		vertices[count++]=particles[r+1][c].pvPosition.y;
        		vertices[count++]=particles[r+1][c].pvPosition.z;
        		
        		vertices[count++]=particles[r][c+1].pvPosition.x;
        		vertices[count++]=particles[r][c+1].pvPosition.y;
        		vertices[count++]=particles[r][c+1].pvPosition.z;
        		
        		vertices[count++]=particles[r][c+1].pvPosition.x;
        		vertices[count++]=particles[r][c+1].pvPosition.y;
        		vertices[count++]=particles[r][c+1].pvPosition.z;
        		
        		vertices[count++]=particles[r+1][c].pvPosition.x;
        		vertices[count++]=particles[r+1][c].pvPosition.y;
        		vertices[count++]=particles[r+1][c].pvPosition.z;
        		
        		vertices[count++]=particles[r+1][c+1].pvPosition.x;
        		vertices[count++]=particles[r+1][c+1].pvPosition.y;
        		vertices[count++]=particles[r+1][c+1].pvPosition.z;
        	}
        }
		return vertices;
	}
	public void initalize()//初始化弹簧质点系统
	{
		for(int r=0;r<=NUMROWS;r++)//对质点网格行进行遍历
		{
			for(int c=0;c<=NUMCOLS;c++)//对质点网格列进列遍历
			{
				particles[r][c] = new Particle();//创建质点对象
				float f;	//声明质点质量临时变量
				if(((r==0)&&(c==0))||((r==NUMROWS)&&(c==NUMCOLS))){
					f = 1;//处在旗帜左上和右下角的质点质量为1
				}
				else if(((r==NUMROWS)&&(c==0))||((r==0)&&(c==NUMCOLS))){
					f=2;//处在旗帜左下和右上角的质点质量为2
				}
				else if(((r==0)&&((c!=0)&&(c!=NUMCOLS)))||((r==NUMROWS)&&((c!=0)&&(c!=NUMCOLS)))){
					f=3;//处在旗帜上下边缘的质点质量为3
				}
				else{
					f=6;//其余质点的质量为6
				}
				f = f/8;//统一将质点质量减小到前面所给值的1/8
				//设置质量,计算质量倒数
				particles[r][c].pfMass = f;
				particles[r][c].pfInvMass = 1/particles[r][c].pfMass;
				
				particles[r][c].pvPosition.x = FLAGPOLERADIUS+c * CSTER;	//根据所在行列计算
				particles[r][c].pvPosition.y = RSTER*NUMROWS/2-r * RSTER; //质点初始位置坐标
				particles[r][c].pvPosition.z = 0;
				if((r==0&&c==0)||(r==NUMROWS&&c==0))
				{//若为左上角或左下角的质点
					particles[r][c].bLocked = true;//将质点设置为锁定点
				}
				else//若不为左上角或左下角的质点
				{
					particles[r][c].bLocked = false;//质点不被锁定
				}
			}
		}
		//初始化弹簧
		int count = 0;								//计数器
		for(int r=0;r<=NUMROWS;r++){					//对质点网格行进行遍历
		  for(int c=0;c<=NUMCOLS;c++){				//对质点网格列进列遍历
			if(c<NUMCOLS){						//初始化横弹簧
				springs[count] = new Spring();			//创建弹簧对象
				springs[count].p1.r = r;				//弹簧连接的第1个质点的行
				springs[count].p1.c = c;				//弹簧连接的第1个质点的列
				springs[count].p2.r = r;				//弹簧连接的第2个质点的行
				springs[count].p2.c = c+1;				//弹簧连接的第2个质点的列
				temp.voluation(particles[r][c].pvPosition);	//将第1个质点的位置设置进临时向量
				temp.sub(particles[r][c+1].pvPosition);	//减去第2个质点的位置
				springs[count].L = temp.module();		//求出弹簧的原始长度
				count++;							//计数器加1
			}
			if(r<NUMROWS){						//初始化竖弹簧
				springs[count] = new Spring();			//创建弹簧对象
				springs[count].p1.r = r;				//弹簧连接的第1个质点的行
				springs[count].p1.c = c;				//弹簧连接的第1个质点的列
				springs[count].p2.r = r+1;				//弹簧连接的第2个质点的行
				springs[count].p2.c = c;				//弹簧连接的第2个质点的列
				temp.voluation(particles[r][c].pvPosition);	//将第1个质点的位置设置进临时向量
				temp.sub(particles[r+1][c].pvPosition);	//减去第2个质点的位置
				springs[count].L = temp.module();		//求出弹簧的原始长度
				count++;							//计数器加1
			}
			if(r<NUMROWS&&c<NUMCOLS){			//初始化左上、右下斜弹簧
				springs[count] = new Spring();			//创建弹簧对象
				springs[count].k = SPRING_SHEAR_CONSTANT;//设置其劲度系数
				springs[count].p1.r = r;				//弹簧连接的第1个质点的行
				springs[count].p1.c = c;				//弹簧连接的第1个质点的列
				springs[count].p2.r = r+1;				//弹簧连接的第2个质点的行
				springs[count].p2.c = c+1;				//弹簧连接的第2个质点的列
				temp.voluation(particles[r][c].pvPosition);	//将第1个质点的位置设置进临时向量
				temp.sub(particles[r+1][c+1].pvPosition); 	//减去第2个质点的位置
				springs[count].L = temp.module();		//求出弹簧的原始长度
				count++;							//计数器加1
			}
			if(r<NUMROWS&&c>0){					//初始化右上、左下斜弹簧
				springs[count] = new Spring();			//创建弹簧对象
				springs[count].k = SPRING_SHEAR_CONSTANT; //设置其劲度系数
				springs[count].p1.r = r;				//弹簧连接的第1个质点的行
				springs[count].p1.c = c;				//弹簧连接的第1个质点的列
				springs[count].p2.r = r+1;				//弹簧连接的第2个质点的行
				springs[count].p2.c = c-1;				//弹簧连接的第2个质点的列
				temp.voluation(particles[r][c].pvPosition);	//将第1个质点的位置设置进临时向量
				temp.sub(particles[r+1][c-1].pvPosition); 	//减去第2个质点的位置
				springs[count].L = temp.module();		//求出弹簧的原始长度
				count++;							//计数器加1
		}}}
		for(int i=0; i<NUMVERTICES*2; i++){			//循环初始化碰撞数组
			collisions[i] = new Collision();				//创建碰撞信息对象
		}
	}
	public void calcForces(){							//计算质点受力的方法
		for(int r=0;r<=NUMROWS;r++){				//对质点网格行进行遍历
			for(int c=0;c<=NUMCOLS;c++){		//对质点网格列进列遍历
				particles[r][c].pvForces.x = 0;		//将质点受力的X分量设置为0
				particles[r][c].pvForces.y = 0;		//将质点受力的Y分量设置为0
				particles[r][c].pvForces.z = 0;		//将质点受力的Z分量设置为0
			}}
		for(int r=0;r<=NUMROWS;r++){				//对质点网格行进行遍历
			for(int c=0;c<=NUMCOLS;c++){			//对质点网格列进行遍历
				if(!particles[r][c].bLocked){				//若该质点没有被锁定
					particles[r][c].pvForces.y += GRAVITY*particles[r][c].pfMass;  //计算重力
					temp.voluation(particles[r][c].pvVelocity);//将质点速度拷贝进临时向量
					temp.normalize();					//获取速度的单位向量
					//计算空气阻力，其方向与质点速度方向相反，其大小与质点速度成正比
					temp.scale(-particles[r][c].pvVelocity.moduleSq()*DRAGCOEFFICIENT);
					particles[r][c].pvForces.add(temp);		//将空气阻力加入总受力
					//产生一个与XOZ平面平行，方向随机的向量
					temp.voluation((float)(Math.random()*1), 0, (float)(Math.random()*0.3f));
					temp.scale((float)(Math.random()*WindForce));//根据当前风力大小产生风力向量
					particles[r][c].pvForces.add(temp);		//将风力加入总受力
				}}}
		for(int i=0;i<NUMSPTINGS;i++){		//遍历所有弹簧计算弹簧弹力
			int r1 = (int) springs[i].p1.r;			//获取弹簧连接的第1个质点的行号
			int c1 = (int) springs[i].p1.c;			//获取弹簧连接的第1个质点的列号
			int r2 = (int) springs[i].p2.r;			//获取弹簧连接的第2个质点的行号
			int c2 = (int) springs[i].p2.c;			//获取弹簧连接的第2个质点的列号
			temp.voluation(particles[r1][c1].pvPosition);//将第一个质点位置复制进临时向量
			temp.sub(particles[r2][c2].pvPosition);	//减去第2个质点的位置
			float pd = temp.module();			//计算出两个质点间的距离
			temp2.voluation(particles[r1][c1].pvVelocity);//将第一个质点速度复制进临时向量
			temp2.sub(particles[r2][c2].pvVelocity);	//减去第2个质点的速度得到弹簧力方向
			float L = springs[i].L;				//获取当前弹簧静止时的长度
			//根据弹簧力公式计算弹簧弹力与阻尼力之和的大小
			float t = -(springs[i].k*(pd-L)+springs[i].d*(temp.dotProduct(temp2)/pd))/pd;
			temp.scale(t);								//得到弹簧力向量
			if(!particles[r1][c1].bLocked){					//若质点1未被锁定
				particles[r1][c1].pvForces.add(temp);			//将弹簧力加入总受力
			}
			if(!particles[r2][c2].bLocked){					//若质点2未被锁定
				temp.scale(-1);		   //计算弹簧力的反向力作为质点2所受弹簧力
				particles[r2][c2].pvForces.add(temp);			//将弹簧力加入总受力
			}
			
		}
	}
	public boolean checkForCollisions()
	{
		int	count = 0;
		boolean	state = false;
		//清除上一轮碰撞信息
		for(int i=0; i<collisions.length; i++){
			collisions[i].r = -1;
		}
		//检测粒子与地面的碰撞
		for(int r=0; r<=NUMROWS; r++)
		{
			for(int c=0; c<=NUMCOLS; c++)
			{
				if(!particles[r][c].bLocked)
				{
					if((particles[r][c].pvPosition.y <= COLLISIONTOLERANCE) && (particles[r][c].pvVelocity.y < 0f))
					{
						state = true;
						collisions[count].r = r;
						collisions[count].c = c;
						collisions[count].n.x = 0.0f;
						collisions[count].n.y = 1.0f;
						collisions[count].n.z = 0.0f;
						count++;
					}
				}
			}
		}		
		//检测粒子与旗杆的碰撞
		for(int r=0; r<=NUMROWS; r++)
		{
			for(int c=0; c<=NUMCOLS; c++)
			{			
				if(!particles[r][c].bLocked)
				{
					//获得此粒子位置距旗杆中线的投影点距离
					float fd = (particles[r][c].pvPosition.x)*(particles[r][c].pvPosition.x)+
								(particles[r][c].pvPosition.z)*(particles[r][c].pvPosition.z);
					temp.voluation(particles[r][c].pvPosition.x,0,particles[r][c].pvPosition.z);
					if((fd <= FLAGPOLERADIUS) && (temp.dotProduct(particles[r][c].pvVelocity)>0f))
					{
						state = true;
						collisions[count].r = r;
						collisions[count].c = c;
						collisions[count].n.voluation(temp);
						collisions[count].n.normalize();
						count++;
					} 
				}
			}
		}
		return state;
	}
	public void resolveCollisions(){						//处理碰撞的方法
		for(int i=0; i<collisions.length; i++){				//遍历碰撞信息对象数组
			if(collisions[i].r != -1){						//若此碰撞信息对象有效
				int r = collisions[i].r;					//获取碰撞对应质点的行号
				int c = collisions[i].c;					//获取碰撞对应质点的列号
				temp.voluation(collisions[i].n);			//获取碰撞面法向量
				//求得法向量方向的速度分量
				temp.scale(temp.dotProduct(particles[r][c].pvVelocity));
				temp2.voluation(particles[r][c].pvVelocity);	//将质点速度拷贝进临时向量2
				temp2.sub(temp);			//减去法向量方向速度分量得切向速度分量
				temp.scale(-KRESTITUTION);			//法向量方向速度乘以负反弹系数
				temp2.scale(FRICTIONFACTOR);		//切向方向速度乘以动摩擦系数
				temp.add(temp2);					//法向量方向速度加切向速度得到总速度
				particles[r][c].pvVelocity.voluation(temp); 	//将总速度设置进质点速度属性
			}
		}
	}
	public void stepSimulation(float dt){			//实现运动方程积分的方法
		calcForces();							//调用calcForces方法计算各个质点的受力
		for(int r=0;r<=NUMROWS;r++){			//对质点网格行进行遍历
			for(int c=0;c<=NUMCOLS;c++){			//对质点网格列进行遍历
				temp.voluation(particles[r][c].pvForces);	//将质点受合力复制进临时向量
				temp.scale(particles[r][c].pfInvMass);		//计算质点总加速度
				particles[r][c].pvAcceleration.voluation(temp);//将加速度记录进质点对象成员
				temp.scale(dt);						//加速度乘以步进时间得到速度变化量
				particles[r][c].pvVelocity.add(temp);		//原速度加上速度变化量得到新速度
				temp.voluation(particles[r][c].pvVelocity);	//将新速度复制进临时向量
				temp.scale(dt);						//新速度乘以步进时间得到位移
				particles[r][c].pvPosition.add(temp);		//原位置加上位移得到新位置
				if(particles[r][c].pvPosition.y<COLLISIONTOLERANCE){//若新位置Y坐标低于地面
					particles[r][c].pvPosition.y=COLLISIONTOLERANCE;//新位置Y坐标等于地面高度
				}
			}
		}
		if(isC){									//若开启了碰撞检测
			if(checkForCollisions()){					//计算碰撞信息
				resolveCollisions();					//处理碰撞
			}
		}
	}
}