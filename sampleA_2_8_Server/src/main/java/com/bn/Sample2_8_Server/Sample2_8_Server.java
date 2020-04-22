package com.bn.Sample2_8_Server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.ServerSocket;
import java.net.Socket;

//服务器端
public class Sample2_8_Server
{
	static ServerSocket sSocket;// ServerSocket的引用
	public static void main(String[] args)
	{
		try
		{
			sSocket=new ServerSocket(8877);//创建ServerSocket对象
			System.out.println("监听8877接口......");//打印信息
			while(true)//服务器启动后一直循环服务于不同的客户端
			{
				Socket socket=sSocket.accept();//创建Socket对象
				DataInputStream diStream=new DataInputStream(socket.getInputStream());
				DataOutputStream dotStream=new DataOutputStream(socket.getOutputStream());
				System.out.println("客户端信息："+diStream.readUTF());
				dotStream.writeUTF("成功连接服务器端");//写入到输出流中
				diStream.close();//关闭输入流
				dotStream.close();//关闭输出流
				socket.close();//关闭Socket套接字
			}
		} 
		catch(Exception e)
		{
			e.printStackTrace();//捕获并打印异常信息
		}
	}
}