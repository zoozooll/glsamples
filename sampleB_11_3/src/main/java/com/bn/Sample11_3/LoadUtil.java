package com.bn.Sample11_3;//声明包
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import android.content.res.Resources;
import android.util.Log;

public class LoadUtil 
{
	
	//从obj文件中加载携带顶点信息的物体，并自动计算每个顶点的平均法向量
    public static GrassObject loadFromFile
    (String fname, Resources r,MySurfaceView mv)
    {
    	//加载后物体的引用
    	GrassObject lo=null;
    	//原始顶点坐标列表--直接从obj文件中加载
    	ArrayList<Float> alv=new ArrayList<Float>();
    	//顶点组装面索引列表--根据面的信息从文件中加载
    	ArrayList<Integer> alFaceIndex=new ArrayList<Integer>();
    	//结果顶点坐标列表--按面组织好
    	ArrayList<Float> alvResult=new ArrayList<Float>();  	
    	//原始纹理坐标列表
    	ArrayList<Float> alt=new ArrayList<Float>();  
    	//结果纹理坐标列表
    	ArrayList<Float> altResult=new ArrayList<Float>();  
    	
    	try
    	{
    		InputStream in=r.getAssets().open(fname);
    		InputStreamReader isr=new InputStreamReader(in);
    		BufferedReader br=new BufferedReader(isr);
    		String temps=null;
    		
    		//扫描文件，根据行类型的不同执行不同的处理逻辑
		    while((temps=br.readLine())!=null) 
		    {//读取一行文本
		    	
		    	String[] tempsa=temps.split("[ ]+");//将文本行用空格符切分
		      	if(tempsa[0].trim().equals("v"))
		      	{//顶点坐标行
		      	    //若为顶点坐标行则提取出此顶点的XYZ坐标添加到原始顶点坐标列表中
		      		alv.add(Float.parseFloat(tempsa[1]));
		      		alv.add(Float.parseFloat(tempsa[2]));
		      		alv.add(Float.parseFloat(tempsa[3]));
		      	}
		      	else if(tempsa[0].trim().equals("vt"))
		      	{//纹理坐标行
		      		//若为纹理坐标行则提取ST坐标并添加进原始纹理坐标列表中
		      		alt.add(Float.parseFloat(tempsa[1]));//提取出S纹理坐标
		      		alt.add(1-Float.parseFloat(tempsa[2])); 	//提取出T纹理坐标
		      	}
		      	else if(tempsa[0].trim().equals("f")) 
		      	{//面数据行
		      		/*
		      		 *若为三角形面行则根据 组成面的顶点的索引从原始顶点坐标列表中
		      		 *提取相应的顶点坐标值添加到结果顶点坐标列表中，同时根据三个
		      		 *顶点的坐标计算出此面的法向量并添加到平均前各个索引对应的点
		      		 *的法向量集合组成的Map中
		      		*/
		      		
		      		int[] index=new int[3];//三个顶点索引值的数组
		      		
		      		//计算第0个顶点的索引，并获取此顶点的XYZ三个坐标	      		
		      		index[0]=Integer.parseInt(tempsa[1].split("/")[0])-1;
		      		float x0=alv.get(3*index[0]);
		      		float y0=alv.get(3*index[0]+1);
		      		float z0=alv.get(3*index[0]+2);
		      		alvResult.add(x0);
		      		alvResult.add(y0);
		      		alvResult.add(z0);		
		      		
		      	    //计算第1个顶点的索引，并获取此顶点的XYZ三个坐标	  
		      		index[1]=Integer.parseInt(tempsa[2].split("/")[0])-1;
		      		float x1=alv.get(3*index[1]);
		      		float y1=alv.get(3*index[1]+1);
		      		float z1=alv.get(3*index[1]+2);
		      		alvResult.add(x1);
		      		alvResult.add(y1);
		      		alvResult.add(z1);
		      		
		      	    //计算第2个顶点的索引，并获取此顶点的XYZ三个坐标	
		      		index[2]=Integer.parseInt(tempsa[3].split("/")[0])-1;
		      		float x2=alv.get(3*index[2]);
		      		float y2=alv.get(3*index[2]+1);
		      		float z2=alv.get(3*index[2]+2);
		      		alvResult.add(x2);
		      		alvResult.add(y2); 
		      		alvResult.add(z2);	
		      		
		      		//记录此面的顶点索引
		      		alFaceIndex.add(index[0]);
		      		alFaceIndex.add(index[1]);
		      		alFaceIndex.add(index[2]);
		      		
		      		//将三角形3个顶点的纹理坐标数据组织到结果纹理坐标列表中
		      		int indexTex=Integer.parseInt(tempsa[1].split("/")[1])-1;//获取纹理坐标编号
		      		//第0个顶点的纹理坐标
		      		altResult.add(alt.get(indexTex*2));
		      		altResult.add(alt.get(indexTex*2+1));
		      	    
		      		indexTex=Integer.parseInt(tempsa[2].split("/")[1])-1;//获取纹理坐标编号
		      		//第1个顶点的纹理坐标
		      		altResult.add(alt.get(indexTex*2));
		      		altResult.add(alt.get(indexTex*2+1));
		      	    
		      		indexTex=Integer.parseInt(tempsa[3].split("/")[1])-1;//获取纹理坐标编号
		      		//第2个顶点的纹理坐标
		      		altResult.add(alt.get(indexTex*2));
		      		altResult.add(alt.get(indexTex*2+1));
		      	}		      		
		    } 
		    
		    //生成顶点数组
		    int size=alvResult.size();
		    float[] vXYZ=new float[size];
		    for(int i=0;i<size;i++)
		    {
		    	vXYZ[i]=alvResult.get(i);
		    }
		    //生成纹理数组
		    size=altResult.size();
		    float[] tST=new float[size];//用于存放结果纹理坐标数据的数组
		    for(int i=0;i<size;i++)
		    {//将纹理坐标数据存入数组
		    	tST[i]=altResult.get(i);
		    }
		    
		    //创建加载物体对象
		    lo=new GrassObject(mv,vXYZ,tST);
    	}
    	catch(Exception e)
    	{
    		Log.d("load error", "load error");
    		e.printStackTrace();
    	}    	
    	return lo;//返回创建的物体对象的引用
    }
}