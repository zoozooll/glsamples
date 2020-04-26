package com.bn.Sample11_4;

import java.io.*;
import java.nio.*;
import android.content.res.Resources;
import android.opengl.GLES30;

//加载顶点Shader与片元Shader的工具类
public class ShaderUtil 
{
   //创建shader程序的方法
   public static int createProgram(String fname,Resources r) 
   { 
        //创建程序
        int program = GLES30.glCreateProgram();        
        //若程序创建成功则向程序中加入顶点着色器与片元着色器
        if (program != 0) 
        {
        	ProgramObject po=loadProgramBinary("shader.bin",r);
        	ByteBuffer buf=ByteBuffer.allocate(po.binLength);
        	buf.put(po.data);
        	buf.position(0);
        	GLES30.glProgramBinary(program, po.binaryFormat, buf, po.binLength);
        }
        return program;
    }
   
   //从sh脚本中加载shader内容的方法
   public static ProgramObject loadProgramBinary(String fname,Resources r)
   {
	    ProgramObject po=null;
	   	try
	   	{
	   		InputStream in=r.getAssets().open(fname);
	   		ObjectInputStream oin=new ObjectInputStream(in);
			po=(ProgramObject)oin.readObject();
			oin.close();		   
	   	}
	   	catch(Exception e)
	   	{
	   		e.printStackTrace();
	   	}    
	   	return po;
   }
}