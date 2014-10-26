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


    // �ı䶥��Ϊ�ĸ������������.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    
    // ��������ϵ�еĶ��㷨��.
    float3 N = mul(input.normal, (float3x3)worldMatrix);
	N = normalize(N);
	//��������ϵ����λ��
    worldPosition = mul(input.position, worldMatrix);
	float3 P = worldPosition.xyz;

	//�Է�����ɫ
	float3 emissive = float3(0.0, 0.0, 0.0);
    
	//���㻷����
    float3 ambient =  float3(0.3, 0.3, 0.3);
	
	//�����������
     float3 L = normalize(float3(-1.0, -1.0, 1.0));
     float diffuseLight = max(dot(N, L), 0);
     float3 diffuse =  diffuseLight;

     //����߹�
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
