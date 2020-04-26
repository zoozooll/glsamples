package com.bn.Sample11_4;

import java.io.*;
import java.nio.*;

import android.content.res.Resources;
import android.opengl.GLES30;
import android.os.Environment;
import android.util.Log;

//加载顶点Shader与片元Shader的工具类
public class ShaderUtil 
{
   //加载制定shader的方法
   public static int loadShader
   (
		 int shaderType, //shader的类型  GLES30.GL_VERTEX_SHADER(顶点)   GLES30.GL_FRAGMENT_SHADER(片元)
		 String source   //shader的脚本字符串
   ) 
   {
	    //创建一个新shader
        int shader = GLES30.glCreateShader(shaderType);
        //若创建成功则加载shader
        if (shader != 0) 
        {
        	//加载shader的源代码
        	GLES30.glShaderSource(shader, source);
            //编译shader
        	GLES30.glCompileShader(shader);
            //存放编译成功shader数量的数组
            int[] compiled = new int[1];
            //获取Shader的编译情况
            GLES30.glGetShaderiv(shader, GLES30.GL_COMPILE_STATUS, compiled, 0);
            if (compiled[0] == 0) 
            {//若编译失败则显示错误日志并删除此shader
                Log.e("ES30_ERROR", "Could not compile shader " + shaderType + ":");
                Log.e("ES30_ERROR", GLES30.glGetShaderInfoLog(shader));
                GLES30.glDeleteShader(shader);
                shader = 0;      
            }  
        }
        return shader;
    }
    
   //创建shader程序的方法
   public static int createProgram(String vertexSource, String fragmentSource) 
   {  
	    //加载顶点着色器
        int vertexShader = loadShader(GLES30.GL_VERTEX_SHADER, vertexSource);
        if (vertexShader == 0) 
        {
            return 0;
        }
        
        //加载片元着色器
        int pixelShader = loadShader(GLES30.GL_FRAGMENT_SHADER, fragmentSource);
        if (pixelShader == 0) 
        {
            return 0;
        }

        //创建程序
        int program = GLES30.glCreateProgram();        
        //若程序创建成功则向程序中加入顶点着色器与片元着色器
        if (program != 0) 
        {
        	//向程序中加入顶点着色器
            GLES30.glAttachShader(program, vertexShader);
            checkGlError("glAttachShader");
            //向程序中加入片元着色器
            GLES30.glAttachShader(program, pixelShader);
            checkGlError("glAttachShader");
            //============================================
            GLES30.glProgramParameteri(program, GLES30.GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GLES30.GL_TRUE);
            //链接程序
            GLES30.glLinkProgram(program);
            //存放链接成功program数量的数组
            int[] linkStatus = new int[1];
            //获取program的链接情况
            GLES30.glGetProgramiv(program, GLES30.GL_LINK_STATUS, linkStatus, 0);
            //若链接失败则报错并删除程序
            if (linkStatus[0] != GLES30.GL_TRUE) 
            {
                Log.e("ES30_ERROR", "Could not link program: ");
                Log.e("ES30_ERROR", GLES30.glGetProgramInfoLog(program));
                GLES30.glDeleteProgram(program);
                program = 0;
            }
            else//若链接成功则将程序的二进制存起来
            {
            	//获取编译后程序的字节数
            	int[] params=new int[1];
            	GLES30.glGetProgramiv(program,GLES30.GL_PROGRAM_BINARY_LENGTH, params, 0);
            	System.out.println("len:"+params[0]);
            	//最大缓冲大小，也就是做多可以写入的着色器二进制码字节数量
            	int bufSize=params[0];
            	//实际二进制码长度
            	int[] binLength=new int[1];
            	//二进制码格式
            	int[] binaryFormat=new int[1];
            	//用于存放二进制码的缓冲
            	ByteBuffer buf=ByteBuffer.allocate(bufSize);
            	
            	GLES30.glGetProgramBinary 
            	(
            		program, 
            		bufSize, 			
            		binLength, 
            		0,				//长度数据在binLength数组中的偏移量
            		binaryFormat, 
            		0, 				//二进制码格式在binaryFormat数组中的偏移量
            		buf
            	);
            	System.out.println("binLength="+binLength[0]);
            	System.out.println("binaryFormat="+binaryFormat[0]);
            	ProgramObject po=new ProgramObject(binLength[0],binaryFormat[0],buf.array());
            	exportProgramBinary(po);
            }
        }
        return program;
    }
   
   public static void exportProgramBinary(ProgramObject po)
   {
	   String path=Environment.getExternalStorageDirectory().getAbsolutePath();	   
	   try
	   {
		   FileOutputStream fout=new FileOutputStream(path+"/shader.bin");		   
		   ObjectOutputStream oout=new ObjectOutputStream(fout);
		   oout.writeObject(po);
		   oout.close();
		   
	   }
	   catch(Exception e)
	   {
		   e.printStackTrace();
	   }
	   System.out.println("out ok"+path);
   }
    
   //检查每一步操作是否有错误的方法
   public static void checkGlError(String op) 
   {
        int error;
        while ((error = GLES30.glGetError()) != GLES30.GL_NO_ERROR) 
        {
            Log.e("ES30_ERROR", op + ": glError " + error);
            throw new RuntimeException(op + ": glError " + error);
        }
   }
   
   //从sh脚本中加载shader内容的方法
   public static String loadFromAssetsFile(String fname,Resources r)
   {
   	String result=null;    	
   	try
   	{
   		InputStream in=r.getAssets().open(fname);
			int ch=0;
		    ByteArrayOutputStream baos = new ByteArrayOutputStream();
		    while((ch=in.read())!=-1)
		    {
		      	baos.write(ch);
		    }      
		    byte[] buff=baos.toByteArray();
		    baos.close();
		    in.close();
   		result=new String(buff,"UTF-8"); 
   		result=result.replaceAll("\\r\\n","\n");
   	}
   	catch(Exception e)
   	{
   		e.printStackTrace();
   	}    	
   	return result;
   }
}