package com.bn.ms3d.core;
import java.io.IOException;
import com.bn.ms3d.io.SmallEndianInputStream;
public class MS3DHeader{//MS3D文件头信息
	String id;
	int version;	
	private MS3DHeader(String id, int version){
		this.id = id;
		this.version = version;
	}
	//从指向ms3d文件的流中读取文件头信息并生成MS3DHeader对象
	public static MS3DHeader load(SmallEndianInputStream is) throws IOException{
		return new MS3DHeader(is.readString(10), is.readInt());
	}
	public String getId(){
		return id;
	}
	public int getVersion(){
		return version;
	}}