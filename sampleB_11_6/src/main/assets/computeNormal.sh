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
layout( std140, binding=4 ) readonly buffer Pos  
{
   pos Positions[ ]; 
}; 
//使用一个buffer缓存块来包含波动后的法向量数据
layout( std140, binding=5 ) buffer Nor  
{
   normal Normals[ ]; 
}; 
 
//输入布局限定符声明一个1X1X1的本地工作组----gl_WorkGroupSize
layout( local_size_x = 1, local_size_y = 1, local_size_z = 1 ) in;

//知道三角形三个点的坐标，计算三角形面法向量的方法
vec3 calNormal(vec3 a,vec3 b,vec3 c)
{
   vec3 vab=b-a;//求a点到b点的向量
   vec3 vac=c-a;//求a点到c点的向量
   return normalize(cross(vab,vac));  //返回最终法向量值
}

void main()                         
{//主函数           
	//算出当前格子左上角顶点的索引            
   	uint indexTemp=gl_NumWorkGroups.x*gl_WorkGroupID.y+gl_WorkGroupID.x;
    
    //三角形编号指南
	//0---1
	//| / |
	//3---2
    
    //若当前顶点不是最后一列，且不是最后一行
    if(gl_WorkGroupID.x<(gl_NumWorkGroups.x-uint(1))&&gl_WorkGroupID.y<(gl_NumWorkGroups.y-uint(1)))
    {
       //计算0-3-1三角形法向量
       //逆时针a点
       vec4 a=Positions[indexTemp].locationA;     
       //逆时针b点
       vec4 b=Positions[indexTemp+gl_NumWorkGroups.x].locationA;
       //逆时针c点
       vec4 c=Positions[indexTemp+uint(1)].locationA;
       //计算此三角形法向量
       vec3 normal=calNormal(a.xyz,b.xyz,c.xyz);
       //给三个顶点记录法向量
       Normals[indexTemp].normalA.xyz+=normal;
       Normals[indexTemp+gl_NumWorkGroups.x].normalA.xyz+=normal;
       Normals[indexTemp+uint(1)].normalA.xyz+=normal;
       
       //计算1-3-2三角形法向量
       //逆时针a点
       a=Positions[indexTemp+uint(1)].locationA;     
       //逆时针b点
       b=Positions[indexTemp+gl_NumWorkGroups.x].locationA;
       //逆时针c点
       c=Positions[indexTemp+uint(1)+gl_NumWorkGroups.x].locationA;
       //计算此三角形法向量
       normal=calNormal(a.xyz,b.xyz,c.xyz);
       //给三个顶点记录法向量
       Normals[indexTemp+uint(1)].normalA.xyz+=normal;
       Normals[indexTemp+gl_NumWorkGroups.x].normalA.xyz+=normal;
       Normals[indexTemp+uint(1)+gl_NumWorkGroups.x].normalA.xyz+=normal;
    }
}