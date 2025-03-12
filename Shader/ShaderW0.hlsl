cbuffer constants : register(b0)
{
    float3 Offset;
    float Radius;
    //float3 Color; // �߰��� �÷� ��
}

struct VS_INPUT
{
    float4 position : POSITION; // Input position from vertex buffer
    float4 color : COLOR; // Input color from vertex buffer
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
    float4 color : COLOR; // Color to pass to the pixel shader
};

PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;
    
    input.position.xy *= Radius;
    //output.position = float4(input.position.xyz * Radius, 1.0) + float4(Offset, 0.0);
    
    output.position = float4(Offset, 0) + input.position;
    output.color = input.color;
    /*
    // ���� ���� ���� üũ
    if (Color.x >= 0.0f)
        output.color = float4(Color, 1.0); // ������ ���� ����
    else
        output.color = input.color; // ���� �������� ����
*/
    return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    return input.color;
}
