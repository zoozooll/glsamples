package com.bn.Sample11_9;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;

public class FontUtil 
{
	static int cIndex=0;//恅掛囀�椸壨�
	static final float textSize=40;//趼极腔湮苤
	static int R=255;//賒捩綻伎籵耋腔硉
	static int G=255;//賒捩蟯伎籵耋腔硉
	static int B=255;//賒捩懦伎籵耋腔硉
	public static Bitmap generateWLT(String[] str,int width,int height)
	{//汜傖恅掛恇燴芞腔源楊
		Paint paint=new Paint();//斐膘賒捩勤砓
		paint.setARGB(255, R, G, B);//扢离賒捩晇伎
		paint.setTextSize(textSize);//扢离趼极湮苤
		paint.setTypeface(null);
		paint.setFlags(Paint.ANTI_ALIAS_FLAG);//湖羲蕨撾喘ㄛ妏趼极晚埽嫖賑
		Bitmap bmTemp=Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
		Canvas canvasTemp = new Canvas(bmTemp);//跦擂硌隅腔弇芞斐膘賒票
		for(int i=0;i<str.length;i++)//餅秶絞ヶ恇燴芞勤茼腔藩俴恅趼
		{
			canvasTemp.drawText(str[i], 0, textSize*i+(i-1)*5, paint);
		}
		return bmTemp;//殿隙餅秶腔釬峈恇燴芞腔弇芞
	}
	static String[] content=
	{
		"梊諦覤綸荍ㄛ挔像邞悕隴﹝",
		"窅偽桽啞鎮ㄛ鴘穖�衚儽ョ�",
		"坋祭伀珨�芄甭尾鼤鄘譝苤�",
		"岈賸痳畟�奴界豰婻縳踼�﹝",
		"玿徹陓鍬窊ㄛ迕膛洏ヶ筵﹝",
		"蔚笵遉紾漸ㄛ厥蘩�偕鍑齱�",
		"��戚苂�酷童疥槬懇團竺寣�",
		"桉豪嫉��綴ㄛ砩ァ匼巍汜﹝",
		"寰梊閨踢曈ㄛ漯策珂涾儐﹝",
		"ロ⑦媼袕尪ㄛ嬧禎湮褽傑﹝",
		"軝侚狨嘎眅ㄛ祥紼岍奻荎﹝",
		"阰夔抎憖狟ㄛ啞忑怮哱冪﹝",
	};
	
	public static String[] getContent(int length,String[] content)
	{//鳳�＞葆佬倇�趼睫揹杅郪腔源楊
		String[] result=new String[length+1];//斐膘趼睫揹杅郪
		for(int i=0;i<=length;i++)
		{
			result[i]=content[i];//蔚絞ヶ剒猁腔囀�楎醙輮�郪
		}
		return result;
	}
	
	public static void updateRGB()//呴儂莉汜賒捩晇伎硉腔源楊
	{
		R=(int)(255*Math.random());//呴儂莉汜賒捩綻伎籵耋硉
		G=(int)(255*Math.random());//呴儂莉汜賒捩蟯伎籵耋硉
		B=(int)(255*Math.random());//呴儂莉汜賒捩懦伎籵耋硉
	}
}