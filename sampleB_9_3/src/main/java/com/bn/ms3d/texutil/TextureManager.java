package com.bn.ms3d.texutil;
import java.util.LinkedHashMap;
import java.util.Map;
import com.bn.ms3d.io.BitmapReader;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.opengl.GLES30;
import android.opengl.GLUtils;

//纹理管理器
public class TextureManager{	 
	Map<String, Integer> textures = new LinkedHashMap<String, Integer>();
	Resources res;	
	public TextureManager(Resources res){
		this.res=res;
	}
	//添加新纹理的方法
	public void addTexture(String texName, String fileName){
		if(!textures.keySet().contains(texName)){
			Bitmap bitmap = null;
			try{
				bitmap = BitmapReader.load(fileName,res);
		        int[] textures = new int[1];
				GLES30.glGenTextures(
						1,          //产生的纹理id的数量
						textures,   //纹理id的数组
						0           //偏移量
				);    
		        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textures[0]);
				GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_NEAREST);
				GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D,GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR);
				GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_CLAMP_TO_EDGE);
				GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_CLAMP_TO_EDGE);
			        GLUtils.texImage2D
		        (
		        		GLES30.GL_TEXTURE_2D,   //纹理类型，在OpenGL ES中必须为GL10.GL_TEXTURE_2D
		        		0, 					  //纹理的层次，0表示基本图像层，可以理解为直接贴图
		        		bitmap, 			  //纹理图像
		        		0					  //纹理边框尺寸
		        );
			    
			    bitmap.recycle();				
				this.textures.put(texName, textures[0]);						
			} 
			catch(Exception e) 
			{
				e.printStackTrace();
			}}}
		//获取指定名称的纹理对应的纹理id的方法
	public final int getTexture(String texName){
		Integer tex = this.textures.get(texName); 
		return (tex != null)?tex.intValue():0;
	}
	//绘制前调用使用指定纹理的方法
	public final void fillTexture(String texName){
		int texture = this.getTexture(texName);
		if(texture != 0){
			//启用2D纹理
			GLES30.glEnable(GLES30.GL_TEXTURE_2D);			
			//绑定纹理
	        GLES30.glActiveTexture(GLES30.GL_TEXTURE0);
	        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texture);
		}}
	//释放指定名称的纹理资源
	public final void dispose(String texName){
		if(textures.keySet().contains(texName)){
			GLES30.glDeleteTextures(1, new int[]{this.textures.get(texName)}, 0);
			System.gc();
		}}}