package com.bn.Sample11_3A;


public class GrassGroup {
	GrassObject grass[];
	public int num;//总的小草棵数
	public float random[];
	//单位平移量
	float unitSize=0.3f;
	int col[];
	int row[];
	float[] cx_Angle;//绕y轴--朝向
	float[] zt_Angle;//绕x轴--姿态（弯曲程度）
	public float[] jbVertex;//颜色渐变的纹理坐标数据
	public GrassGroup(GrassObject object,int num)
	{
		this.num=num;
		//每行多少个---小草总数为2的n次幂
		int colCount=(int) Math.sqrt(num); 
		
		grass=new GrassObject[num];//GrassObject对象
		col=new int[num];//创建存放行信息的对象数组
		row=new int[num];//创建存放行信息的对象数组
		random=new float[num];//创建存放随机数的对象数组
		cx_Angle=new float[num];//绕y轴--朝向
		zt_Angle=new float[num];//绕x轴--姿态（弯曲程度）
		jbVertex=new float[num*2];
		
		for(int i=0;i<num;i++)
		{//生成小草数组
			grass[i]=object;
			random[i]=1+(float) Math.random();//生成0~1的随机数
			
			//计算列号
			col[i]=i%colCount;
			//计算行号
			row[i]=i/colCount;
			
			jbVertex[2*i]=(float)col[i]/colCount;
			jbVertex[2*i+1]=(float)row[i]/colCount;
			cx_Angle[i]=(float) Math.random()*360;//绕y轴--朝向
			zt_Angle[i]=(float) Math.random()*30;//绕x轴--姿态（弯曲程度）
		}
	}
	public void drawGroup(int texId,int jbTexId)
	{
		for(int i=0;i<num;i++)
		{
			MatrixState.pushMatrix();
			MatrixState.translate(unitSize*col[i]*random[i]-unitSize, 0, unitSize*row[i]*random[i]-unitSize);
			MatrixState.rotate(cx_Angle[i], 0, 1, 0);//绕y轴--朝向
			MatrixState.rotate(zt_Angle[i], 1, 0, 0);//绕x轴--姿态（弯曲程度）
			//若加载的物体不为空则绘制物体
			if(grass[i]!=null)
			{
				grass[i].drawSelf(texId,jbTexId,new float[]{jbVertex[2*i],jbVertex[2*i+1]});
			}
			MatrixState.popMatrix();
		}
	}

}