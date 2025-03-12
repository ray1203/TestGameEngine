cbuffer constants : register(b0)
{
    row_major float4x4 MVP;
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
    output.position = mul(input.position, MVP);
    output.color = input.color;
    return output;
}
