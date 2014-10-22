////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer LightMaterialBuffer
{
    float3 cameraPosition;
    float padding;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;


    // 改变顶点为四个分量其次坐标.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    
    // 世界坐标系中的顶点法向.
    float3 N = mul(input.normal, (float3x3)worldMatrix);
	N = normalize(N);
	//世界坐标系顶点位置
    worldPosition = mul(input.position, worldMatrix);
	float3 P = worldPosition.xyz;

	//自发射颜色
	float3 emissive = float3(0.0, 0.0, 0.0);
    
	//计算环境光
    float3 ambient =  float3(0.3, 0.3, 0.3);
	
	//计算漫反射光
     float3 L = normalize(float3(-1.0, -1.0, 1.0));
     float diffuseLight = max(dot(N, L), 0);
     float3 diffuse =  diffuseLight;

     //计算高光
     float3 V = normalize(cameraPosition - P);
     float3 H = normalize(L + V);
     float specularLight = pow(max(dot(N, H), 0), 5.0);
 
      if (diffuseLight <= 0) 
	      specularLight = 0;
      float3 specular =  specularLight;

	  output.color.xyz = emissive + ambient + diffuse + specular;
	 // float3 tt = float3(1.0, 0.0, 0.0);
	 //  output.color.xyz = float3(1.0, 0.0, 0.0);
	  output.color.w = 1.0f;

    return output;
}
