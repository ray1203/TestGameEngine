cbuffer constants : register(b0)
{
    float3 Offset;
    float Pad;
    float Radius;
    float3 Color; // 추가된 컬러 값
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
    
    output.position = float4(input.position.xyz * Radius, 1.0) + float4(Offset, 0.0);
    
    // 색상 변경 여부 체크
    if (Color.x >= 0.0f)
        output.color = float4(Color, 1.0); // 설정된 색상 적용
    else
        output.color = input.color; // 기존 무지개색 유지

    return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
    return input.color;
}
