#version 310 es
precision mediump float;//给出默认的浮点精度
struct pos
{
   vec4 locationA; //顶点位置
};

struct normal
{
   vec4 normalA; //顶点法向量
};
//使用一个buffer缓存块来包含波动后的顶点数据
layout( std140, binding=4 ) buffer Pos  
{
   pos Positions[ ]; 
}; 
//使用一个buffer缓存块来包含波动后的法向量数据
layout( std140, binding=5 ) writeonly buffer Nor  
{
   normal Normals[ ]; 
}; 
 
//输入布局限定符声明一个1X1X1的本地工作组----gl_WorkGroupSize
layout( local_size_x = 1, local_size_y = 1, local_size_z = 1 ) in;

uniform vec2 bx1;	//波心1
uniform float bc1;	//波长1
uniform float zf1;  //振幅1
uniform float qsj1; //起始角1

uniform vec2 bx2;	//波心2
uniform float bc2;	//波长2
uniform float zf2;  //振幅2
uniform float qsj2; //起始角2

uniform vec2 bx3;	//波心3
uniform float bc3;	//波长3
uniform float zf3;  //振幅3
uniform float qsj3; //起始角3

//计算一个波对指定点的高度扰动
float calHdr
(
	vec2 bx,	//波心
	float bc,	//波长
	float zf,	//振幅
	float qsj,	//起始角
	vec2 ddxz	//被扰动的顶点xz坐标
)
{
    //计算与波心的距离
    float dis=distance(ddxz,bx);
    //算出角度跨度
    float angleSpan=dis*2.0*3.1415926/bc;
    //计算此波对此顶点的振幅扰动
    float hrd=sin(angleSpan+qsj)*zf;
    //返回高度扰动值
    return hrd;
}

void main()                         
{ //主函数           
	//算出顶点的索引            
   	uint indexTemp=gl_NumWorkGroups.x*gl_WorkGroupID.y+gl_WorkGroupID.x;
    //取出顶点的位置
    vec4 positionTemp=Positions[indexTemp].locationA;
    //计算三个波对顶点的高度扰动值并叠加
    Positions[indexTemp].locationA.y=calHdr(bx1,bc1,zf1,qsj1,positionTemp.xz)+
                                     calHdr(bx2,bc2,zf2,qsj2,positionTemp.xz)+
                                     calHdr(bx3,bc3,zf3,qsj3,positionTemp.xz);
    //此顶点的法向量清零，以备下一轮计算法向量
    Normals[indexTemp].normalA=vec4(0.0,0.0,0.0,0.0);
}