package com.bn.Sample2_8_Client;//声明包名

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;


import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class Sample2_8_ClientActivity extends Activity//创建继承Activity的主控制类
{
    @Override
    public void onCreate(Bundle savedInstanceState)//重写的onCreate方法
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);//跳转到主界面
        Button button=(Button)findViewById(R.id.button);//获得Button按钮的引用
        button.setOnClickListener//为Button按钮添加监听器
        (
        	new OnClickListener()
        	{
				public void onClick(View v)	//重写的onClick方法
				{
					new Thread()
					{
						public void run()
						{
							connectServer();//调用connectServer连接服务器
						}
					}.start();
					
				}
        	}
        );
    }
    //连接服务器端的方法
    public void connectServer()//自定义的连接服务器的方法
    {
    	
    	String serverIp="10.16.189.20";//声明服务器端IP
    	try
    	{
    		Socket socket=new Socket(serverIp,8877); //创建Socket套接字，发出连接请求
    		DataInputStream din=new DataInputStream(socket.getInputStream());
			DataOutputStream dout=new DataOutputStream(socket.getOutputStream());
			EditText et=(EditText)this.findViewById(R.id.et);//获得EditText输入对话框对象
			String tempStr=et.getText().toString();//获取该对话框中的信息
			dout.writeUTF(tempStr);//将信息写入到输出流中
			TextView tv=(TextView)this.findViewById(R.id.tv);//获得TextView的对象
			tv.setText(din.readUTF());//将输入流中的数据在TextView中显示
			din.close();//关闭输入流
			dout.close();//关闭输chu流
			socket.close();//关闭Socket套接字
    	}
    	catch(Exception e)
    	{
    		e.printStackTrace();//捕获并打印异常信息
    	}
    }
}