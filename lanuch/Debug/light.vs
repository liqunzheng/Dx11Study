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

//const buffer���Ϊ4 float�ı��������򴴽�const buffer��fail
cbuffer LightMaterialBuffer
{
    float4 lightPosition; //��Դλ��
	float4 lightColor;   //��Դ��ɫ
	float4 globalAmbient; //��Դ�Ļ����ⷴ��ϵ��
    float4 cameraPosition; //�������λ��
	float4 Ke;  //���ʵ��Է���
	float4 Ka;  //���ʵĻ�����ϵ��
	float4 Kd;  //���ʵ�������ϵ��
	float4 Ks;  //���ʵĸ߹�ϵ��
	float3 lightDirection; //ƽ�йⷽ��
	float shininess; //�߹�ָ��
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
	float4 emissive = Ke;
    
	//���㻷����
    float4 ambient = Ka * globalAmbient;
	
	//�����������
     float3 L = -normalize(lightDirection);
     float diffuseLight = max(dot(N, L), 0);
     float4 diffuse = Kd * lightColor * diffuseLight;

	 //����߹�
     float3 V = normalize(cameraPosition.xyz - P);
     float3 H = normalize(L + V);
     float specularLight = pow(max(dot(N, H), 0), shininess);
 
      if (diffuseLight <= 0) 
	      specularLight = 0;
      float4 specular = Ks * lightColor * specularLight;

	  output.color = emissive + ambient + diffuse + specular;

      return output;
}
