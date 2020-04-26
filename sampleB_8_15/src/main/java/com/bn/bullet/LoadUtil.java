package com.bn.bullet;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import android.content.res.Resources;
import android.util.Log;

public class LoadUtil 
{
	//求两个向量的叉积
	public static float[] getCrossProduct(float x1,float y1,float z1,float x2,float y2,float z2)
	{		
		//求出两个矢量叉积矢量在XYZ轴的分量ABC
        float A=y1*z2-y2*z1;
        float B=z1*x2-z2*x1;
        float C=x1*y2-x2*y1;
		
		return new float[]{A,B,C};
	}
	
	//向量规格化
	public static float[] vectorNormal(float[] vector)
	{
		//求向量的模
		float module=(float)Math.sqrt(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);
		return new float[]{vector[0]/module,vector[1]/module,vector[2]/module};
	}
	
	public static float[] m_vertices;//顶点数组
	public static int m_numsVer;//顶点数组长度
	public static int[] m_indices;//顶点索引数组
	public static int m_numsInd;//顶点索引数组长度
	
	public static float[] m_normals;//法向量数组
	public static int m_numsNor;//法向量数组长度

	public static float[] m_tex;//纹理数组
	public static int m_numsTex;//纹理数组长度
	
	//从obj文件中加载携带顶点信息的物体，并自动计算每个顶点的平均法向量
	//获取顶点数据，顶点索引数据  纹理数据  纹理索引数据   并不计算平均法向量
    public static void loadFromFileWd(String fname, Resources r)
    {
    	//原始顶点坐标列表--直接从obj文件中加载
    	ArrayList<Float> alv=new ArrayList<Float>();
    	//顶点组装面索引列表--根据面的信息从文件中加载
    	ArrayList<Integer> alFaceIndex=new ArrayList<Integer>();
    	//原始纹理坐标列表
    	ArrayList<Float> alt=new ArrayList<Float>(); 
    	//纹理索引 列表
    	ArrayList<Integer> alFaceIndex_tex=new ArrayList<Integer>();
    	
    	try
    	{
    		InputStream in=r.getAssets().open(fname);
    		InputStreamReader isr=new InputStreamReader(in);
    		BufferedReader br=new BufferedReader(isr);
    		String temps=null;
    		
    		//扫面文件，根据行类型的不同执行不同的处理逻辑
		    while((temps=br.readLine())!=null) 
		    {
		    	//用空格分割行中的各个组成部分
		    	String[] tempsa=temps.split("[ ]+");
		      	if(tempsa[0].trim().equals("v"))
		      	{//此行为顶点坐标
		      	    //若为顶点坐标行则提取出此顶点的XYZ坐标添加到原始顶点坐标列表中
		      		alv.add(Float.parseFloat(tempsa[1]));
		      		alv.add(Float.parseFloat(tempsa[2]));
		      		alv.add(Float.parseFloat(tempsa[3]));
		      	}
		      	else if(tempsa[0].trim().equals("f"))
		      	{//此行为三角形面
		      		int[] index=new int[3];//三个顶点索引值的数组
		      		//计算第0个顶点的索引，并获取此顶点的XYZ三个坐标	      		
		      		index[0]=Integer.parseInt(tempsa[1].split("/")[0])-1;
		      	    //计算第1个顶点的索引，并获取此顶点的XYZ三个坐标	  
		      		index[1]=Integer.parseInt(tempsa[2].split("/")[0])-1;
		      	    //计算第2个顶点的索引，并获取此顶点的XYZ三个坐标	
		      		index[2]=Integer.parseInt(tempsa[3].split("/")[0])-1;
		      		//记录此面的顶点索引
		      		alFaceIndex.add(index[0]);
		      		alFaceIndex.add(index[1]);
		      		alFaceIndex.add(index[2]);
		      		
		      		index[0] = Integer.parseInt(tempsa[1].split("/")[1])-1;
		      		index[1] = Integer.parseInt(tempsa[2].split("/")[1])-1;
		      		index[2] = Integer.parseInt(tempsa[3].split("/")[1])-1;
		      		alFaceIndex_tex.add(index[0]);
		      		alFaceIndex_tex.add(index[1]);
		      		alFaceIndex_tex.add(index[2]);
		      	}
		      	else if(tempsa[0].trim().equals("vt"))
		      	{
		      		//若为纹理坐标行则提取ST坐标并添加进原始纹理坐标列表中
		      		float sl=Float.parseFloat(tempsa[1]);
		      		float tl=Float.parseFloat(tempsa[2]);
		      		float t=1.0f-tl;
		      		float s=sl;
		      		alt.add(s);
		      		alt.add(t);
		      	}
		    } 
		    
		    //生成顶点数组
		    m_numsVer=alv.size();
		    m_vertices=new float[m_numsVer];
		    for(int i=0;i<m_numsVer;i++)
		    {
		    	m_vertices[i]=alv.get(i);
		    }
		    
		    //生成顶点索引数组
		    m_numsInd=alFaceIndex.size();
		    m_indices=new int[m_numsInd];
		    for(int i=0;i<m_numsInd;i++)
		    {
		    	m_indices[i]=alFaceIndex.get(i);
		    }
		    
		    //纹理数组
		    int m_numsTex_temp = alt.size();
		    float[] m_tex_temp = new float[m_numsTex_temp];
		    for(int i = 0; i < m_numsTex_temp; i++)
		    {
		    	m_tex_temp[i] = alt.get(i);
		    }
		    
		    //纹理索引数组
		    int m_numsInd_tex = alFaceIndex_tex.size();
		    int[] m_indices_tex = new int[m_numsInd_tex];
		    for(int i = 0; i < m_numsInd_tex; i++)
		    {
		    	m_indices_tex[i] = alFaceIndex_tex.get(i);
		    }
		    
		    m_numsTex = m_numsInd_tex*2;
		    m_tex = new float[m_numsTex];
		    for(int i = 0; i < m_numsInd_tex; i++)
		    {
		    	int index = m_indices_tex[i];
		    	m_tex[i*2+0] = m_tex_temp[index*2];
		    	m_tex[i*2+1] = m_tex_temp[index*2+1];
//		    	System.out.println(m_tex[i*2+0]+" "+m_tex[i*2+1]);
		    }
//		    System.out.println("m_numsTex = "+m_numsTex);
    	}
    	catch(Exception e)
    	{
    		Log.d("load error", "load error");
    		e.printStackTrace();
    	}    	
    	return;
    }
	



}