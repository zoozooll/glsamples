package com.bn.Sample11_4;

import java.io.*;

public class ProgramObject implements Serializable
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 231714197949345483L;
	//实际二进制码长度
	int binLength;
	//二进制码格式
	int binaryFormat;
	//二进制数据
	byte[] data;
	
	public ProgramObject(int binLength,int binaryFormat,byte[] data)
	{
		this.binLength=binLength;
		this.binaryFormat=binaryFormat;
		this.data=data;
	}
}